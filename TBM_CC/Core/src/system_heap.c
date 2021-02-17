//#include "../include/system_heap.h"
#include "system_heap.h"

uint16_t        g_free_blocks[0x10];
uint16_t        g_used_blocks[0x10];
vheap_group *   heapg_head = ((vheap_group *)0);
vheap_group *   heapg_current = ((vheap_group *)0);
vheap_block *   heapb_current = ((vheap_block *)0);
volatile void * free_heap_ptr = (volatile void *)MEM_START;
heap_region     designated_heap;

/** TODO:
 * 1. Use list of unavailable memory blocks while also keeping
 *    track of the current largest available memory block.
 *
 * 2. Possibly use a list of available memory blocks aswell
 *
 * 3. Look into faster memory mapping, maybe with a partial free list solution?
 *    https://en.wikipedia.org/wiki/Free_list
 *
 * */
void *
malloc_(uint16_t obj_size)
{
  if (obj_size == 0x0) {
    return NULL;
  }

  uint8_t mem_not_found = 0x1;
  heapg_current = heapg_head;

  if ((heapg_head->_size) < 1) {
    // Why is READ_HEAP_FREEBLOCKS() reading 0 free blocks?!?
    return NULL;
  }

  void * free_block_ptr = NULL;
  for (; mem_not_found || (heapg_current != ((vheap_group *)0));) {
    if (READ_HEAP_FREEBLOCKS(heapg_current) > 0) {
      // return NULL; // DEBUG REMOVE LATER
      free_block_ptr = __find_mem__(heapg_current, obj_size);
      mem_not_found = (free_block_ptr == NULL);
    }
    heapg_current = heapg_current->next;
    // return ((void*)0x1010); // DEBUG REMOVE LATER
  }
  return free_block_ptr;
}

void
free(void * ptr)
{
  __remove_block__((vheap_block *)(((vuint8_t *)(ptr)) - HB_HEADER_SIZE));
  ptr = NULL;
}

/**
 * @brief
 *
 * NOTE: Have tested it and it seems to work as expected, I have tested up to 2
 * generated heaps and it works. This leaves hte question why the head heap
 * still reads free _block as 0 when I read it later on.
 *
 * How I tested it: I changed the return types to int and depending on if I
 * retrurned a 0 or 1, I let the main function run the blinky example. Basically,
 * I use the blinker function to notify me if things worked as expected or not.
 *
 * I did this to check on the first and the second call to __gen_single_heapg__
 * Everything worked as expected, which was frankly unexpected and leads me to
 * believe it might have something to do with how pointers are accessed later
 * on, or synchronization issues could also be at hand here.
 *
 */
void
__gen_single_heapg__(uint32_t start_addr_heap, uint8_t idx)
{
  vheap_group * temp = ((vheap_group *)0);
  if (heapg_current == ((vheap_group *)0)) {
    heapg_head = (vheap_group *)(start_addr_heap);
    temp = heapg_current = heapg_head;
  } else {
    temp = (heapg_current->next = (vheap_group *)(start_addr_heap));
    temp->prev = heapg_current;
  }
  temp->_size = 0x80008000;
  temp->_blocks = 0x00010000;
  temp->group_id = idx;

  heapb_current = HBHG_INCR_ADDR(temp, HG_HEADER_SIZE + HG_HEADER_SIZE);
  heapb_current->data_size = MAX_HB_DATA_SIZE;
  heapb_current->prev = (vheap_block *)NULL;
  heapb_current->next = (vheap_block *)NULL;
  SET_BLOCK_FREE(heapb_current);
  SET_GROUP_ID(heapb_current->id_n_freed, idx);
}

void
__init_ram_heap__()
{
  uint8_t  idx = 0;
  uint32_t current_addr = MEM_START;
  for (; idx < 16; idx++) {
    if ((IOMUXC_GPR_GPR17 >> (2 * idx) & 0x3) == 0x2) {
      __gen_single_heapg__(MEM_START + (idx * 0x8000), idx);
    }
  }
}

/**
 * @brief Tries to find free memory
 * @param heap_g THe heap group to scan
 * @param size the requested object size
 * @return Returns either a NULL ptr or a valid pointer
 **/
void *
__find_mem__(vheap_group * heap_g, uint16_t size)
{
  vheap_block * cu_b = HG_HEAD_BLOCK(heap_g); // Point to first block
  vheap_block * new_b = ((vheap_block *)0);
  vheap_block * end_b = HBHG_INCR_ADDR(heap_g, READ_HEAP_TOTAL(heap_g));
  return NULL; // DEBUG REMOVE LATER

  for (; (READ_BLOCK_FREE(cu_b)) || (cu_b != (vheap_block *)NULL);
       cu_b = cu_b->next) {
    vuint16_t * size_ptr = &(cu_b->data_size);
    if (*size_ptr == size) {
      // if size is exactly what is left; decrement one in _blocks [free]
      g_free_blocks[heap_g->group_id]--;
      // Increment used blocks
      g_used_blocks[heap_g->group_id]++;

      SET_BLOCK_USED(cu_b);
      return VOID_INCR_ADDR(cu_b, HB_HEADER_SIZE);
    }

    if (*size_ptr > size) {
      // "cut" block to appropriate size
      *size_ptr = size;

      // "make" new free block
      new_b = HBHG_INCR_ADDR(cu_b, HB_HEADER_SIZE + (*size_ptr));
      new_b->id_n_freed = cu_b->id_n_freed; // Copy id and free bits
      new_b->prev = cu_b;
      new_b->next = cu_b->next;
      cu_b->next = new_b;
      SET_BLOCK_USED(cu_b);

      // Increment used blocks
      g_used_blocks[heap_g->group_id]++;
      return VOID_INCR_ADDR(cu_b, HB_HEADER_SIZE);
    }
  }
  return NULL;
}

/**
 * @brief To remove holes, squish blocks together
 * Pretty naive method of copying, I might need to look into creating a virtual
 * address manager instead of wasting processing power on copying bytes
 * backwards in a long interation. Having virtual addresses I could get away
 * with not actually copying data backwards when I perform compactations
 *
 * @param heap_g Heap group to compact.
 **/
void
__compactation__(vheap_group * heap_g)
{
  // iterate through block and compact it
  vheap_block * hb_cptr = (vheap_block *)(heap_g + HG_HEADER_SIZE);
  vheap_block * hb_eptr = (vheap_block *)(heap_g + 0x8000);
  // looping through the blocks
  for (; (BLOCK_END(hb_cptr)) != hb_eptr;) {
    /**If Free, swap it forward.  */
    if (READ_BLOCK_FREE(hb_cptr)) {
      __coalesce_front__(hb_cptr);
      __data_swap_next__(hb_cptr);
    }
    hb_cptr += hb_cptr->data_size; // Incremetning to next block
  }
}

/**
 * @brief Free Block of memory
 * Clearing data isn't actually needed
 **/
void
__remove_block__(vheap_block * heap_b)
{
  SET_BLOCK_FREE(heap_b);
  uint8_t group_id = READ_BLOCK_GID(heap_b->id_n_freed);
  g_free_blocks[group_id] += 1;
  g_used_blocks[group_id] -= 1;
  __coalesce__(heap_b);
}

/**
 * @brief Coalesce Blocks of memory recursively, first frontwards from the end
 * to given block, then backwards from the given block to the start
 *
 * NOTE: Relinking the coalesced block with new 'prev' and 'next' pointers
 *       happens within the __coalesce__ internal functions __coalesce_front__
 *       and __coalesce_back__
 **/
void
__coalesce__(vheap_block * heap_b)
{
  if (READ_BLOCK_FREE(heap_b->next)) {
    __coalesce_front__(heap_b);
  }
  if (READ_BLOCK_FREE(heap_b->prev)) {
    heap_b = __coalesce_back__(heap_b);
  }
}

/**
 * @brief Coalesce Blocks of memory recursively, frontwards
 * @param heap_b pointer to block to coalesce forward from
 **/
void
__coalesce_front__(vheap_block * heap_b)
{
  if (heap_b->next == (vheap_block *)NULL) {
    return;
  }
  if (READ_BLOCK_FREE(heap_b->next)) {
    __coalesce_front__(heap_b->next);
    heap_b->data_size += heap_b->next->data_size;
    heap_b->next->data_size = 0x0;
    g_free_blocks[READ_BLOCK_GID(heap_b->id_n_freed)] -= 1;

    /** Moving next pointer back to starting pointer
     * Base, Next0, Next1, End,
     * Next0->next = Next1->next (End), Base->next = Next0->next (End) */
    heap_b->next = heap_b->next->next;
  }
}

/**
 * @brief Coalesce Blocks of memory recursively, backwards
 * @param heap_b pointer to block to coalesce backward from
 * @return Returns a pointer to the 'prev' pointer
 **/
vheap_block *
__coalesce_back__(vheap_block * heap_b)
{
  if (heap_b->prev == NULL) {
    return (vheap_block *)NULL;
  }
  vheap_block * prev_cpy = heap_b;
  if (READ_BLOCK_FREE(heap_b->prev)) {
    heap_b->prev->data_size += heap_b->data_size;
    heap_b->data_size = 0x0;
    heap_b->prev->next = heap_b->next; // moving next pointer back
    prev_cpy = __coalesce_back__(heap_b->prev);
    g_free_blocks[READ_BLOCK_GID(heap_b->id_n_freed)] -= 1;
  }
  return prev_cpy;
}

void
__data_swap_next__(vheap_block * heap_b)
{
  vheap_block * hb_sptr = ((vheap_block *)0);
  uint16_t      datasize_cpy = 0;
  // Swapping data sizes
  datasize_cpy = heap_b->data_size;
  heap_b->data_size = heap_b->next->data_size;
  heap_b->next->data_size = datasize_cpy;

  // Move data
  vuint8_t * next_data = (vuint8_t *)(heap_b->next + HG_HEADER_SIZE);
  vuint8_t * curr_data = (vuint8_t *)(heap_b + HG_HEADER_SIZE);
  vuint8_t * next_data_end =
      (vuint8_t *)(heap_b->next + HG_HEADER_SIZE + heap_b->next->data_size);
  for (; next_data != next_data_end;) {
    *curr_data = *next_data;
    curr_data += 1;
    next_data += 1;
  }

  // Swapping prev ptrs
  hb_sptr = heap_b->prev;
  heap_b->prev = heap_b->next->prev;
  heap_b->next->prev = hb_sptr;

  // Swapping next ptrs
  hb_sptr = heap_b->next;
  heap_b->next = heap_b->next->next;
  heap_b->next->next = hb_sptr;
}

/**
 * Demonstration of heap behavior:
 * Memory is alloc'd by returning the 1st block large enough for the request.
 * Memory is returned or freed in any convenient order.
 * When two blocks of allocated memory are freed, they are coalesced to form a
 * single block to better meet demand for larger blocks of memory.
 *
 * How to handle fragmentation (GC - Compactation):
 * After a given interval of time (or a threshold amount for fragmentation),
 * then move fragmented memory blocks within a heap next to eachother.
 *
 *
 * Idea:
 * Maybe imitate stack behavior UNTIL a heap is filled, after that free blocks
 * in any order. Have a register of Free'd areas (blocks), and their sizes,
 * within a heap.
 *
 * If a memory block is requested: Only if no available heap has a free memory
 * block that has a size that satisfies the request, then:
 *  Check if any heaps has enough free memory, if there is one:
 *  Pick the first one with enough free total memory and perform compactation
 *
 * Offshoot ideas:
 *  Maybe have a variable compactation_counter which counts amounts of free
 *  operations since the last compactation
 *
 *  Maybe keep track of how many holes a heap has, the heaps with most
 *  fragmentations will be submitted to the scheduled GC compactation
 *
 *  Maybe also count how many bytes are still left from the end of the heap to
 *  the closest non-free block
 **/
