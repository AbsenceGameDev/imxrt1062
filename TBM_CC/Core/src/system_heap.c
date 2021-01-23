#include "../include/system_heap.h"

/** TODO:
 * 1. Make a memory-block structure.
 * 2. Use the memory block structure within mem_alloc()
 * 3. Use list of unavailable memory blocks while also keeping
 *    track of the current largest available memory block.
 *
 * 4. Possibly use a list of available memory blocks aswell
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

#define __set_designated_heap(s_addr, e_addr, frag_s_addr, frag_e_addr)        \
  designated_heap.start_addr_heap = (volatile void *)s_addr;                   \
  designated_heap.end_addr_heap = (volatile void *)e_addr;                     \
  designated_heap.frag_start_addr_heap = (volatile void *)frag_s_addr;         \
  designated_heap.frag_end_addr_heap = (volatile void *)frag_e_addr;

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

// Split the into N amounts of 32KB segments,
// N is based on the totalt sice
#define __gen_heap_group_in_region(start_addr_heap, end_addr_heap, heapg_head) \
  heap_group * heapg_head = (heap_group *)start_addr_heap;                     \
  heapg_head->prev = (head_group *)NULL_ADDR;                                  \
  uint32_t KBSize = (end_addr_heap - start_addr_heap) / 0x400;                 \
  KBSize /= 32;                                                                \
  for (uint8_t i = 1; i < KBSize; i++) {                                       \
    heapg_head->next = heapg_head + 0x8000;                                    \
    heapg_head += (0x8000 * i);                                                \
    heapg_head->free_size = 0x8000;                                            \
    heapg_head->total_size = 0x8000;                                           \
    heapg_head->prev = (heapg_head - 0x8000);                                  \
  }

/** @brief create-heap: create an empty heap */
heap_group *
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
  return (heap_group *)(designated_heap.start_addr_heap);
}
