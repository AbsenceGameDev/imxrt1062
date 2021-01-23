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

void
set_heap_regions()
{
  // Read the currently set rambank config.
  ERAMBankConf ram_bank_conf = (ERAMBankConf)IOMUXC_GPR_GPR17;
  switch (ram_bank_conf) {
    case FLEXRAM_O00_I50_D50:
      designated_heap = {(volatile void * 0x0),
                         (volatile void * 0x0),
                         (volatile void * 0x0),
                         (volatile void * 0x0)};
      return;
    case FLEXRAM_O50_I25_D25:
    case FLEXRAM_O50_I50_D00:
    case FLEXRAM_O50_I00_D50:
      designated_heap = {(volatile void * MEM_START),
                         (volatile void *(MEM_START + 0x00040000)),
                         (volatile void *(MEM_START + 0x00080000)),
                         (volatile void *(MEM_START + 0x000c0000))};
      return;
    case FLEXRAM_OCRAM_ONLY:
      designated_heap = {(volatile void * MEM_START),
                         (volatile void *(MEM_START + 0x00040000)),
                         (volatile void * 0x0),
                         (volatile void * 0x0)};
      return;
    case FLEXRAM_O25_I50_D25:
    case FLEXRAM_O25_I25_D50:
      designated_heap = {(volatile void *(MEM_START + 0x00040000)),
                         (volatile void *(MEM_START + 0x00060000)),
                         (volatile void * 0x0),
                         (volatile void * 0x0)};
      return;
  }
}

/** @brief create-heap: create an empty heap */
heap_group *
create_heap(uint16_t heap_byte_size)
{
  // Placeholder for tomorrow
  return (heap_group *)(designated_heap.start_addr_heap);
}
