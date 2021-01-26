#include "../include/system_heap.h"

/** TODO:
 * 1. Make a memory-block structure.
 * 2. Use the memory block structure within mem_alloc()
 * 3. Use list of unavailable memory blocks while also keeping
 *    track of the current largest available memory block.
 *
 * 4. Possibly use a list of available memory blocks aswell
 *
 * 5. Look into faster memory mapping, maybe with a partial free list solution?
 *    https://en.wikipedia.org/wiki/Free_list
 *
 * */
void *
mem_alloc(uint16_t obj_size)
{
  uint8_t is_enough_space =
      (!((obj_size < 1) || ((free_heap_ptr + (vuint8_t *)obj_size) > MEM_END)));
  (free_heap_ptr += obj_size * is_enough_space);
  return (void *)(((vuint8_t *)free_heap_ptr - (vuint8_t *)obj_size) *
                  is_enough_space);
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
    case FLEXRAM_O00_I50_D50: __set_designated_heap(0x0, 0x0, 0x0, 0x0); return;
    case FLEXRAM_O50_I25_D25:
    case FLEXRAM_O50_I50_D00:
    case FLEXRAM_O50_I00_D50:
      __set_designated_heap(MEM_START,
                            MEM_OFFS(0x00040000),
                            MEM_OFFS(0x00080000),
                            MEM_OFFS(0x000c0000));
      return;
    case FLEXRAM_OCRAM_ONLY:
      __set_designated_heap(MEM_START, MEM_OFFS(0x00080000), 0x0, 0x0);
      return;
    case FLEXRAM_O25_I50_D25:
    case FLEXRAM_O25_I25_D50:
      __set_designated_heap(
          MEM_OFFS(0x00040000), MEM_OFFS(0x00060000), 0x0, 0x0);
      return;
  }
}

/** @brief create-heap: create an empty heap */
void
generate_heap_groups(uint16_t heap_byte_size)
{
  if (designated_heap.start_addr_heap != NULL_ADDR) {
    __gen_heap_group_in_region(designated_heap.start_addr_heap,
                               designated_heap.end_addr_heap,
                               heapg_head);
  }
  if (designated_heap.frag_start_addr_heap != NULL_ADDR) {
    __gen_heap_group_in_region(designated_heap.frag_start_addr_heap,
                               designated_heap.frag_end_addr_heap,
                               heapg2_head);
  }
}

/**
 * typedef struct {
 * heap_block * prev;
 * heap_block * next;
 * size_t       data_size;
 * uint8_t      id_n_freed;
 * } heap_block;
 **/
void
__compactation__(heap_group * heapgroup)
{
  // iterate through block and compact it
  heap_block * hb_sptr = (heap_block *)(heapgroup + HG_HEADER_SIZE);
  heap_block * hb_cptr = (heap_block *)(heapgroup + HG_HEADER_SIZE);
  heap_block * hb_eptr = (heap_block *)(heapgroup + 0x8000);
  uint16_t free_blocks = READ_HEAP_FREE(heapgroup->_size); // maybe not needed

  // looping through the blocks
  for (; (heap_block *)(BLOCK_END(hb_cptr)) != hb_eptr;) {
    __coalesce_front__(hb_cptr);
    hb_cptr += hb_cptr->data_size;
  }
  //(READ_BLOCK_FREE(hb_cptr));

  // handle last block after for loop
  return;
}

/**
 * @brief Free Block of memory
 * Clearing data isn't actually needed
 **/
void
__remove_block__(heap_block * heapblock)
{
  heapblock->id_n_freed = SET_BLOCK_FREE(heapblock);
  uint8_t group_id = READ_GROUP_ID(heapblock->id_n_freed);
  g_free_blocks[group_id] += 1;
  g_used_blocks[group_id] -= 1;
  return;
}

/**
 * @brief Coalesce Blocks of memory recursively, first frontwards from the end
 * to given block, then backwards from the given block to the start
 *
 * @todo Relink the coalesced block with the new 'prev' and 'next' pointers
 **/
void
__coalesce__(heap_block * heap_b)
{
  heap_block * new_next;
  heap_block * new_prev;
  if (READ_BLOCK_FREE(heap_b->next)) {
    new_next = __coalesce_front__(heap_b);
  }
  if (READ_BLOCK_FREE(heap_b->prev)) {
    new_prev = __coalesce_back__(heap_b);
  }
}

/**
 * @brief Coalesce Blocks of memory recursively, frontwards
 * @param heap_b pointer to block to coalesce forward to
 * @return Returns a pointer to the 'next' pointer
 **/
heap_block *
__coalesce_front__(heap_block * heap_b)
{
  if (heap_b->next == (heap_block *)NULL) {
    return (heap_block *)NULL;
  }
  heap_block * next_cpy = heap_b->next;
  if (READ_BLOCK_FREE(heap_b->next)) {
    next_cpy = __coalesce_front__(heap_b->next);
    heap_b->data_size += heap_b->next->data_size;
    heap_b->next->data_size = 0x0;
  }
  return next_cpy;
}

/**
 * @brief Coalesce Blocks of memory recursively, backwards
 * @param heap_b pointer to block to coalesce backward from
 * @return Returns a pointer to the 'prev' pointer
 **/
void
__coalesce_back__(heap_block * heap_b)
{
  if (heap_b->prev == NULL) {
    return (heap_block *)NULL;
  }
  heap_block * prev_cpy = heap_b->prev;
  if (READ_BLOCK_FREE(heap_b->prev)) {
    heap_b->prev->data_size += heap_b->data_size;
    heap_b->data_size = 0x0;
    prev_cpy = __coalesce_back__(heap_b->prev);
  }
  return prev_cpy;
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
