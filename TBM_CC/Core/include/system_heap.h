#ifndef SYSTEM_HEAP_H
#define SYSTEM_HEAP_H

#include "system_memory_map.h"

/**
 * The common operations involving heaps are:
 *
 *Basic
 *    - find-max (or find-min): find a maximum item of a max-heap, or a minimum
 *      item of a min-heap, respectively (a.k.a. peek)
 *    - insert: adding a new key to the heap (a.k.a., push[4])
 *    - extract-max (or extract-min): returns the node of maximum value from a
 *      max heap [or minimum value from a min heap] after removing it from the
 *      heap (a.k.a., pop[5])
 *    - delete-max (or delete-min): removing the root node of a max heap
 *      (or min heap), respectively
 *    - replace: pop root and push a new key. More efficient than pop followed
 *      by push, since only need to balance once, not twice,
 *      and appropriate for fixed-size heaps.[6]
 *
 *Creation
 *    - create-heap: create an empty heap
 *    - heapify: create a heap out of given array of elements
 *    - merge (union): joining two heaps to form a valid new heap containing all
 *      the elements of both, preserving the original heaps.
 *    - meld: joining two heaps to form a valid new heap containing all the
 *      elements of both, destroying the original heaps.
 *
 *Inspection
 *    - size: return the number of items in the heap.
 *    - is-empty: return true if the heap is empty, false otherwise.
 *
 *Internal
 *    - increase-key or decrease-key:
 *      updating a key within a max- or min-heap, respectively
 *    - delete: delete an arbitrary node
 *      (followed by moving last node and sifting to maintain heap)
 *    - sift-up: move a node up in the tree, as long as needed; used to restore
 *      heap condition after insertion. Called "sift" because node moves up the
 *      tree until it reaches the correct level, as in a sieve.
 *    - sift-down: move a node down in the tree, similar to sift-up; used to
 *      restore heap condition after deletion or replacement.
 **/

/**
 * TODO: Configure OCRAM for READ and WRITE access, chapter 30.3 - 30.5
 *
 * 30.4 Advanced Features:
 * This section describes some advanced features designed to avoid timing issues
 * when the on-chip RAM is working at high frequency.
 * All of the features can be disabled/enabled by programming the corresponding
 * fields of the General Purpose Register (IOMUXC.GPR3) bits [3:0] in the IOMUXC
 *
 * For the normal OCRAM,
 * Read Data Wait Atate is configurable via IOMUXC.GPR3[0]
 * Read Address Pipeline is configurable via IOMUXC.GPR3[1].
 * Write Data Pipeline is configurable via IOMUXC.GPR3[2]
 * Write Address Pipeline is configurable via IOMUXC.GPR3[3]
 *
 * NOTE: There are no programmable registers in this block; however, OCRAM
 * configurable bits can be found in the IOMUX Controller (IOMUXC) general
 * purpose registers found here:
 * TrustZone bits: IOMUXC_GPR10
 *
 * NOTE: 31.3   FLEXRAM Functional description
 * FLEXRAM converts AXI and TCM interface signals to RAM interface signals
 * and implements mux control for OCRAM , DTCM and ITCM access to on-chip RAM.
 *
 * NOTE: 31.3.1 FLEXRAM Interface Conversion
 *
 * NOTE: 31.3.2 FLEXRAM RAM Bank Allocation
 * OCRAM, DTCM and ITCM share 512 KB of on-chip RAM. SW can configure from 0
 * to 512 KB full RAM array size for OCRAM, DTCM and ITCM by IOMUX GPR16
 * and GPR17. The RAM size step is 32KB. The allocated RAM bank can be not
 * overlapped for OCRAM, DTCM and ITCM.
 *
 * Each RAM bank has 2 bits bank configuration from IOMUX GPR17:
 * GPR_FLEXRAM_BANK_CFG[2n+1 : 2n], where n = 0, 1, ..., 15
 * 00 = Not used
 * 01 = OCRAM
 * 10 = DTCM
 * 11 = ITCM
 *
 * Using above schema to maek some preset examples below
 * == 1010 1111 0101 0101 1010 1111 0101 0101
 * == 0xaf55af55 // OCRAM 50%, ITCM 25%, DTCM 25%
 * == 0xa5ffa5ff // ITCM 50%, OCRAM 25%, DTCM 25%
 * == 0xf5aaf5aa // DTCM 50%, OCRAM 25%, ITCM 25%
 *
 * Flexram source select and initialization of TCM (tightly couple memory):
 * |======================|===================================================|
 * |Field                 |  Description                                      |
 * |======================|===================================================|
 * |31–7, CM7_INIT_VTOR   | Vector table offset register out of reset. See Arm|
 * |                      | v7-M Architecture Ref. Manual for more information|
 * |                      | about the vector table offset register (VTOR).    |
 * |                      | (p.601, B3.2.5, in DDI0403E_d...)                 |
 * |======================|===================================================|
 * |6–3-                  | This field is reserved.                           |
 * |                      | Reserved                                          |
 * |======================|===================================================|
 * |2,FLEXRAM_BANK_CFG_SEL| FlexRAM bank config source select                 |
 * |                      | 0: use fuse value to config                       |
 * |                      | 1: use FLEXRAM_BANK_CFG to config                 |
 * |======================|===================================================|
 * |1,INIT_DTCM_EN        | DTCM enable initialization out of reset.          |
 * |                      | NOTE:When a TCM is enabled, the corresponding     |
 * |                      | CFG*TCMSZ register must NOT be set to 0'b0000     |
 * |                      | 0: DTCM is disabled                               |
 * |                      | 1: DTCM is enabled                                |
 * |======================|===================================================|
 * |0,INIT_ITCM_EN        | ITCM enable initialization out of reset.          |
 * |                      | NOTE: When a TCM is enabled, the corresponding    |
 * |                      | CFG*TCMSZ register must NOT be set to 0'b0000     |
 * |                      | 0: ITCM is disabled                               |
 * |                      | 1: ITCM is enabled                                |
 * |======================|===================================================|
 **/

typedef enum
{
  FLEXRAM_OCRAM_ONLY = 0x55555555, // OCRAM 100%
  FLEXRAM_OCRAM_MAIN = 0xaf55af55, // OCRAM 50%, ITCM 25%, DTCM 25%
  FLEXRAM_ITCM_MAIN = 0xa5ffa5ff, // ITCM 50%, OCRAM 25%, DTCM 25%
  FLEXRAM_DTCM_MAIN = 0xf5aaf5aa, // DTCM 50%, OCRAM 25%, ITCM 25%
  FLEXRAM_NO_OCRAM = 0xafafafaf, // DTCM 50%, ITCM 50%, OCRAM 00%
  FLEXRAM_NO_DTCM = 0xf5f5f5f5, // ITCM 50%, OCRAM 50%, DTCM 00%
  FLEXRAM_NO_ITCM = 0xa5a5a5a5 // DTCM 50%, OCRAM 50%, ITCM 00%
} ERAMBankConf;

typedef enum
{
  FUSE_VAL_CONF,
  GPR17_BANK_CFG
} EFlexSrcSelect;

typedef enum
{
  TCM_DISABLE,
  TCM_ENABLE
} EInitTCM;

void
ram_bank_presets(ERAMBankConf requested_config);

// OCRAM FLEXRAM (FLEXIBLE MEMORY ARRAY, will use for heap space)
#define MEM_START (SYSMEM_OCRAM_FLEX_E - 0x00040000)
#define MEM_END SYSMEM_OCRAM_FLEX_E // reserving 128kb for mem_alloc
volatile void * free_heap_ptr = (volatile void *)MEM_START;

/** TODO: Actually write the mem_alloc function */
void *
mem_alloc(uint16_t obj_size);

#endif // SYSTEM_HEAP_H