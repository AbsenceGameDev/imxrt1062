#include "../include/system_heap.h"

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
  uint8_t mem_not_found = 0x1;
  heapg_current = heapg_head;
  void * free_block_ptr = NULL;

  for (; mem_not_found || (heapg_current != (heap_group *)NULL);) {
    if (READ_HEAP_FREEBLOCKS(heapg_current) > 0) {
      free_block_ptr = __find_mem__(heapg_current, obj_size);
      mem_not_found = (free_block_ptr == NULL);
    }
    heapg_current = heapg_current->next;
  }
  return free_block_ptr;
}

void
free(void * ptr)
{
  __remove_block__((heap_block *)(((vuint8_t *)(ptr)) - HB_HEADER_SIZE));
  ptr = NULL;
}

void
ram_bank_presets(ERAMBankConf requested_config)
{
  // Setting BANK_CFG field in GPR16 to set flexram-bank config source
  // to gpr17 instead of fuse value
  IOMUXC_GPR_GPR16 |= FLEXRAM_BANK_CFG_SEL(GPR17_BANK_CFG);

  // ENABLE DTCM and ITCM?
  switch (requested_config) {
    case FLEXRAM_O50_I50_D00: // FLEXRAM_NO_DTCM
      IOMUXC_GPR_GPR16 |= FLEXRAM_DTCM_EN(TCM_DISABLE);
      IOMUXC_GPR_GPR16 |= FLEXRAM_ITCM_EN(TCM_ENABLE);
      break;
    case FLEXRAM_O50_I00_D50: // FLEXRAM_NO_ITCM
      IOMUXC_GPR_GPR16 |= FLEXRAM_DTCM_EN(TCM_ENABLE);
      IOMUXC_GPR_GPR16 |= FLEXRAM_ITCM_EN(TCM_DISABLE);
      break;
    case FLEXRAM_OCRAM_ONLY:
      IOMUXC_GPR_GPR16 |= FLEXRAM_ITCM_EN(TCM_DISABLE);
      IOMUXC_GPR_GPR16 |= FLEXRAM_DTCM_EN(TCM_DISABLE);
    default:
      IOMUXC_GPR_GPR16 |= FLEXRAM_DTCM_EN(TCM_ENABLE);
      IOMUXC_GPR_GPR16 |= FLEXRAM_ITCM_EN(TCM_ENABLE);
      break;
  }
  //  Set all OCRAM space available for all access types
  //  (secure/non-secure/user/supervisor).
  SET_OCRAM_TRUSZONE(TZ_DISABLE);
  return (IOMUXC_GPR_GPR17 = requested_config);
}

void
set_heap_regions()
{
  // Read the currently set rambank config.
  ERAMBankConf ram_bank_conf = (ERAMBankConf)IOMUXC_GPR_GPR17;
  switch (ram_bank_conf) {
    case FLEXRAM_O00_I50_D50:
    case FLEXRAM_O25_I25_D50:
    case FLEXRAM_O50_I00_D50:
      __set_designated_heap(MEM_START,
                            MEM_OFFS(0x00020000),
                            MEM_OFFS(0x00040000),
                            MEM_OFFS(0x00080000));
      return;
    case FLEXRAM_OCRAM_ONLY: // Slow heap
      __set_designated_heap(MEM_START, MEM_OFFS(0x00080000), 0x0, 0x0);
      return;
    case FLEXRAM_O25_I50_D25:
    case FLEXRAM_O50_I25_D25:
    case FLEXRAM_O50_I50_D00:
      __set_designated_heap(
          MEM_OFFS(0x00040000), MEM_OFFS(0x00060000), 0x0, 0x0);
      return;
  }
}

/** @brief generates empty heap groups */
void
generate_heap_groups(uint16_t heap_byte_size)
{
  if (designated_heap.start_addr_heap != NULL_ADDR) {
    __gen_heap_group_in_region(designated_heap.start_addr_heap,
                               designated_heap.end_addr_heap);
  }
  if (designated_heap.frag_start_addr_heap != NULL_ADDR) {
    __gen_heap_group_in_region(designated_heap.frag_start_addr_heap,
                               designated_heap.frag_end_addr_heap);
  }
}

/**
 * @brief Tries to find free memory
 * @param heap_g THe heap group to scan
 * @param size the requested object size
 * @return Returns either a NULL ptr or a valid pointer
 **/
void *
__find_mem__(heap_group * heap_g, uint16_t size)
{
  heap_block * cu_b = HG_HEAD_BLOCK(heap_g); // Point to first block
  heap_block * new_b;
  heap_block * end_b = HBHG_INCR_ADDR(heap_g, READ_HEAP_TOTAL(heap_g));

  for (; (READ_BLOCK_FREE(cu_b)) || (cu_b != (heap_block *)NULL);
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
__compactation__(heap_group * heap_g)
{
  // iterate through block and compact it
  heap_block * hb_cptr = (heap_block *)(heap_g + HG_HEADER_SIZE);
  heap_block * hb_eptr = (heap_block *)(heap_g + 0x8000);
  // looping through the blocks
  for (; (heap_block *)(BLOCK_END(hb_cptr)) != hb_eptr;) {
    /**If Free, swap it forward.  */
    if (READ_BLOCK_FREE(hb_cptr)) {
      __coalesce_front__(hb_cptr);
      __data_swap_next__(hb_cptr);
    }
    hb_cptr += hb_cptr->data_size; // Incremetning to next block
  }
  return;
}

/**
 * @brief Free Block of memory
 * Clearing data isn't actually needed
 **/
void
__remove_block__(heap_block * heap_b)
{
  SET_BLOCK_FREE(heap_b);
  uint8_t group_id = READ_BLOCK_GID(heap_b->id_n_freed);
  g_free_blocks[group_id] += 1;
  g_used_blocks[group_id] -= 1;
  __coalesce__(heap_b);
  return;
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
__coalesce__(heap_block * heap_b)
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
__coalesce_front__(heap_block * heap_b)
{
  if (heap_b->next == (heap_block *)NULL) {
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
  return;
}

/**
 * @brief Coalesce Blocks of memory recursively, backwards
 * @param heap_b pointer to block to coalesce backward from
 * @return Returns a pointer to the 'prev' pointer
 **/
heap_block *
__coalesce_back__(heap_block * heap_b)
{
  if (heap_b->prev == NULL) {
    return (heap_block *)NULL;
  }
  heap_block * prev_cpy = heap_b;
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
__data_swap_next__(heap_block * heap_b)
{
  heap_block * hb_sptr;
  uint16_t     datasize_cpy;
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
