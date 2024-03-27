/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2024, MIT-License included in project toplevel dir
 */

#include "sys/heap.h"

extern char     __heap_start;
extern char     __heap_end;
volatile char * __malloc_heap_start = &__heap_start;
volatile char * __malloc_heap_end = &__heap_end;

uint16_t        g_free_blocks[0x10];
uint16_t        g_used_blocks[0x10];
vheap_group *   heapg_head = ((vheap_group *)0);
vheap_group *   heapg_tail = ((vheap_group *)0);
vheap_group *   heapg_current = ((vheap_group *)0);
vheap_block *   heapb_current = ((vheap_block *)0);
volatile void * free_heap_ptr = (volatile void *)MEM_START;
heap_region     designated_heap;

/**
 * @brief   lightweight memory allocation, uses OCFlexRAM for simplicity
 *
 * @param   obj_size
 * @return  void*  The allocated memory or NULL
 *
 * @todo
 * 1. Use list of unavailable memory blocks while also keeping
 *    track of the current largest available memory block.
 * 2. Possibly use a list of available memory blocks aswell
 * 3. Look into faster memory mapping, maybe with a partial free list solution?
 *    https://en.wikipedia.org/wiki/Free_list
 *
 * @bug (FIXED) Alignment bug was causing memory to fail allocating properly, manulaly fixed aligment but mi
 */
void *
malloc_(uint16_t obj_size)
{
  if (obj_size == 0x0 || heapg_current == NULL) 
  {
    return NULL;
  }

  uint8_t mem_not_found = 0x1;
  // heapg_current = heapg_head;

  if (heapg_current->_size < 1 && heapg_current->_size != 0x80008000) 
  {
    return NULL;
  }

  void * free_block_ptr = NULL;
  for (; mem_not_found || (heapg_current != ((vheap_group *)0));) 
  {
    if (READ_HEAP_FREEBLOCKS(heapg_current) > 0) 
    {
      free_block_ptr = __find_mem__(heapg_current, obj_size);
      mem_not_found = (free_block_ptr == NULL);
    }
    heapg_current = heapg_current->next;
  }
  return free_block_ptr;
}

void
free_(void * ptr)
{
  __remove_block__((vheap_block *)(((vuint8_t *)(ptr)) - HB_HEADER_SIZE));
  ptr = NULL;
}

void
__gen_single_heapg__(uint32_t start_addr_heap, uint8_t idx)
{
  vheap_group * temp = ((vheap_group *)0);
  if (heapg_current == ((vheap_group *)0)) 
  {
    heapg_head = (vheap_group *)(start_addr_heap);
    temp = heapg_current = heapg_tail = heapg_head;
  } 
  else 
  {
    temp = (heapg_current->next = (vheap_group *)(start_addr_heap));
    temp->prev = heapg_current;
  }

  temp->_size = 0x80008000;
  temp->_blocks = 0x00010000;
  temp->group_id = idx;

  heapb_current = HBHG_INCR_ADDR(temp, HG_HEADER_SIZE + HG_HEADER_SIZE);
  heapb_current->max_data_size = heapb_current->curr_data_size = MAX_HB_DATA_SIZE;
  heapb_current->prev = (vheap_block *)NULL;
  heapb_current->next = (vheap_block *)NULL;
  SET_BLOCK_FREE(heapb_current);
  SET_GROUP_ID(heapb_current->id_n_freed, idx);
}

void
__init_ram_heap__()
{
  uint8_t idx = 0;
  for (; idx < 16; idx++) 
  {
    if ((IOMUXC_GPR_GPR17 >> (2 * idx) & 0x3) == 0x2) 
    {
      __gen_single_heapg__(MEM_START + (idx * 0x8000), idx);
    }
  }

  // heapg_current = heapg_tail;
  heapg_current = heapg_head; // Point back to the head 
  heapg_head->_size = 0x80008000;
}

/**
 * @brief Tries to find free memory
 * @param heap_g THe heap group to scan
 * @param size the requested object size
 * @return Returns either a NULL ptr or a valid pointer
 **/
void *
__find_mem__(vheap_group * heap_g, uint16_t requested_size)
{
  vheap_block * current_block = HG_HEAD_BLOCK(heap_g); // Point to first block
  vheap_block * new_block = ((vheap_block *)0);
  vheap_block * end_block = HBHG_INCR_ADDR(heap_g, READ_HEAP_TOTAL(heap_g));

  for (; (current_block != (vheap_block *)NULL) && current_block != end_block; current_block = current_block->next) 
  { 
    if (READ_BLOCK_FREE(current_block) == 0x0) { continue; } 

    uint16_t current_offset = current_block->max_data_size - current_block->curr_data_size;
    vuint16_t* curr_size_ptr = &(current_block->curr_data_size);
    uint16_t requested_size_delta = *curr_size_ptr - requested_size;

    // Check if requested size is exactly what is left
    if (requested_size_delta == 0) 
    {
      // decrement one in _blocks [free]
      g_free_blocks[heap_g->group_id]--;
      // Increment one in _blocks [used]
      g_used_blocks[heap_g->group_id]++;

      new_block->prev = current_block;
      new_block->curr_data_size = 0x0;
      new_block->next = current_block->next;
      current_block->next = new_block;      

      SET_BLOCK_USED(current_block);

      // Return the allocated memory
      return VOID_INCR_ADDR(current_block, HB_HEADER_SIZE);
    }

    // Check if requested size is greater than what is left
    // @todo Accumulate all these free partials and put into a special heap group when memory gets too fragmented
    if (requested_size_delta > 0) { continue; }

    // Check if requested size is less than what is left
    if (requested_size_delta > 0) 
    {
      // Adjust the current block size and return the allocated memory
      new_block->curr_data_size = *curr_size_ptr - requested_size;
      return VOID_INCR_ADDR(current_block, current_offset);
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
 * 
 **/
void
__compactation__(vheap_group * heap_g)
{
  // iterate through block and compact it
  vheap_block * hb_cptr = (vheap_block *)(heap_g + HG_HEADER_SIZE);
  vheap_block * hb_eptr = (vheap_block *)(heap_g + 0x8000);
  // looping through the blocks
  for (; (BLOCK_END_FULL(hb_cptr)) != hb_eptr ;) 
  {
    /**If Free, swap it forward.  */
    if (READ_BLOCK_FREE(hb_cptr)) 
    {
      /** @note This only handles full blocks that are flaggeds as free, not partial ones that might be flagged as non free **/
      /** @todo Handle partially free blocks **/

      __coalesce_neighbour_front__(hb_cptr);
      __data_swap_next__(hb_cptr);
    }

    /** Incrementing to next block */
    /** @todo consider using offsetof() ? */
    hb_cptr += hb_cptr->max_data_size; 
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
 * to given block, then backwards from the given block to the as far back it can go without hitting a used block
 *
 * NOTE: Relinking the coalesced block with new 'prev' and 'next' pointers
 *       happens within the __coalesce__ internal functions __coalesce_neighbour_front__
 *       and __coalesce_neighbour_back__
 **/
void
__coalesce__(vheap_block * heap_b)
{
  if (READ_BLOCK_FREE(heap_b->next)) 
  {
    __coalesce_neighbour_front__(heap_b);
  }

  if (READ_BLOCK_FREE(heap_b->prev)) 
  {
    heap_b = __coalesce_neighbour_back__(heap_b);
  }
}

/**
 * @brief Coalesce Blocks of memory recursively, from the furthest reachable block the current block
 * @param heap_b pointer to block to coalesce into
 * 
 * @note In essence this actually coalesces, backwards from the blocks in front of the current block
 **/
void
__coalesce_neighbour_front__(vheap_block * heap_b)
{
  if (heap_b->next == (vheap_block *)NULL) 
  {
    return;
  }

  if (READ_BLOCK_FREE(heap_b->next)) 
  {
    __coalesce_neighbour_front__(heap_b->next); // Recursively coalesce, must start with the furthest block.
    heap_b->curr_data_size += heap_b->next->curr_data_size;
    heap_b->next->curr_data_size = 0x0;
    g_free_blocks[READ_BLOCK_GID(heap_b->id_n_freed)] -= 1;

    /** Moving next pointer back to starting pointer
     * Base, Next0, Next1, End,
     * Next0->next = Next1->next (End), Base->next = Next0->next (End) */
    heap_b->next = heap_b->next->next;
  }
}

/**
 * @brief Coalesce conescutive blocks of memory recursively, backwards
 * @param heap_b pointer to block to coalesce backward from
 * @return Returns a pointer to the 'prev' pointer
 **/
vheap_block*
__coalesce_neighbour_back__(vheap_block * heap_b)
{
  if (heap_b->prev == NULL) 
  {
    return (vheap_block *)NULL;
  }
  vheap_block * prev_cpy = heap_b;  

  if (READ_BLOCK_FREE(heap_b->prev)) 
  {
    heap_b->prev->curr_data_size += heap_b->curr_data_size; // Coalesce data sizes
    heap_b->curr_data_size = 0x0;                           // Clear current data size
    heap_b->prev->next = heap_b->next;                      // Moving next pointer back
    prev_cpy = __coalesce_neighbour_back__(heap_b->prev);   // Recursively coalesce
    g_free_blocks[READ_BLOCK_GID(heap_b->id_n_freed)] -= 1; // Decrement free blocks, @todo(2024/03) need to take into account partially free blocks
  }
  return prev_cpy;
}

void
__data_swap_next__(vheap_block * heap_b)
{
  __data_swap__(heap_b, heap_b->next);
}

void
__data_swap__(vheap_block* heap_b_src, vheap_block * heap_b_target)
{
  vheap_block * hb_sptr = ((vheap_block *)0);
  uint16_t      datasize_cpy = 0;

  // Swapping data sizes
  datasize_cpy = heap_b_src->curr_data_size;
  heap_b_src->curr_data_size = heap_b_target->curr_data_size;
  heap_b_target->curr_data_size = datasize_cpy;

  // Addresses to swap data between
  vuint8_t * curr_data       = (vuint8_t *)(heap_b_src + HG_HEADER_SIZE);
  vuint8_t * target_data     = (vuint8_t *)(heap_b_target + HG_HEADER_SIZE);
  vuint8_t * target_data_end = BLOCK_END_FULL_VU8(heap_b_target); // End of target block
  
  // Swap data
  for (; target_data != target_data_end; )
  {
    *curr_data = *target_data;
    curr_data += 1;
    target_data += 1;
  }

  // Swapping prev ptrs
  hb_sptr = heap_b_src->prev;
  heap_b_src->prev = heap_b_target->prev;
  heap_b_target->prev = hb_sptr;

  // Swapping next ptrs
  hb_sptr = heap_b_src->next;
  heap_b_src->next = heap_b_target->next;
  heap_b_target->next = hb_sptr;
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
 *  operations since the last compactation and use that to be able to control the max frequency of compactation events
 *
 *  Maybe keep track of how many holes a heap has, the heaps with most
 *  fragmentations will be submitted to the scheduled GC compactation
 *
 *  Maybe also count how many bytes are still left from the end of the heap to
 *  the closest non-free block
 **/
