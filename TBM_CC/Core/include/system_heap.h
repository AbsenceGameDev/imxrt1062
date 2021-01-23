#ifndef SYSTEM_HEAP_H
#define SYSTEM_HEAP_H

#include "system_memory_map.h"

#define NULL_ADDR (volatile void * 0x0)

/**
 * @brief: OCRAM TrustZone (TZ) enable macro.
 *
 * 0x0: The TrustZone feature is disabled. Entire OCRAM space is available for
 * all access types (secure/non-secure/user/supervisor).
 *
 * 0x1: The TrustZone feature is enabled. Access to address in the range
 * specified by[ENDADDR : STARTADDR] follows the execution mode access policy
 * described in CSU chapter */
#define SET_OCRAM_TRUSZONE(x) IOMUXC_GPR_GPR10 = ((x & 0x1) << 8)

// OCRAM FLEXRAM (FLEXIBLE MEMORY ARRAY, will use for heap space)
#define MEM_START SYSMEM_OCRAM_FLEX_S // S - 0x00040000)
#define MEM_END SYSMEM_OCRAM_FLEX_E // reserving 128kb for mem_alloc
volatile void * free_heap_ptr = (volatile void *)MEM_START;
#define MEM_OFFS(x) (MEM_START + x)

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

/**
 * @brief RAM Bank presets enum. Each enum holds a value chosen to configure
 * banks according to 11.4.18 at p.367 in IMXRT1060 processor ref. manual
 * @param FLEXRAM_OCRAM_ONLY  0x55555555 / OCRAM 100%
 * @param FLEXRAM_O50_I25_D25 0xaf55af55 / OCRAM 50%, ITCM 25%, DTCM 25%
 * @param FLEXRAM_O25_I50_D25 0xa5ffa5ff / ITCM 50%, OCRAM 25%, DTCM 25%
 * @param FLEXRAM_O25_I25_D50 0xf5aaf5aa / DTCM 50%, OCRAM 25%, ITCM 25%
 * @param FLEXRAM_O00_I50_D50 0xafafafaf / DTCM 50%, ITCM 50%, OCRAM 00%
 * @param FLEXRAM_O50_I50_D00 0xf5f5f5f5 / ITCM 50%, OCRAM 50%, DTCM 00%
 * @param FLEXRAM_O50_I00_D50 0xa5a5a5a5 / DTCM 50%, OCRAM 50%, ITCM 00%
 **/
typedef enum
{
  FLEXRAM_OCRAM_ONLY = 0x55555555,
  FLEXRAM_O50_I25_D25 = 0xaf55af55,
  FLEXRAM_O25_I50_D25 = 0xa5ffa5ff,
  FLEXRAM_O25_I25_D50 = 0xf5aaf5aa,
  FLEXRAM_O00_I50_D50 = 0xafafafaf,
  FLEXRAM_O50_I50_D00 = 0xff55ff55,
  FLEXRAM_O50_I00_D50 = 0xaa55aa55
} ERAMBankConf;

typedef struct {
  volatile void * start_addr_heap;
  volatile void * end_addr_heap;
  volatile void * frag_start_addr_heap;
  volatile void * frag_end_addr_heap;
} heap_region;
heap_region designated_heap;

typedef uint8_t heap_group_t;

typedef struct {
  heap_group * prev;
  heap_group * next;
  heap_group_t group;
  size_t       total_size;
  size_t       free_size;
  size_t       block_count;
} heap_group;

typedef struct {
  heap_block * prev;
  heap_block * next;
  size_t       data_size;
  uint8_t      freed;
} heap_block;

typedef enum
{
  FUSE_VAL_CONF,
  GPR17_BANK_CFG
} EFlexSrcSelect;

typedef enum
{
  FALSE,
  TRUE
} _bool_;

typedef enum
{
  TCM_DISABLE,
  TCM_ENABLE
} EInitTCM;

typedef enum
{
  TZ_DISABLE = 0x0,
  TZ_ENABLE = 0x1
} ETrustZoneState;

/**
 * @brief RAM Bank presets function.
 * NOTE: Set the rambanks before allocating heap
 * @param [in] requested_config RAM Bank Preset enum.
 * Refer to the ERAMBankConf enum for further documentation.
 **/
void
ram_bank_presets(ERAMBankConf requested_config);

/**
 * @brief Allocates an address region for heap based on the RAM Bank configs.
 * @return returns a struct which holds the now reserved heap region
 **/
void
set_heap_regions();

/** @brief Heap creation funcs. */

/** @brief Generates 32kb heap groups based on the FLEXRAM Config */
heap_group *
generate_heap_groups();

/** @brief create-heap: create an empty heap */
heap_group *
create_heap(uint16_t heap_byte_size);

/** @brief heapify: create a heap out of given array of elements */
heap_group *
heapify(void * context);

/** @brief merge (union): joining two heaps to form a valid new heap containing
 * all the elements of both, preserving the original heaps. */
heap_group *
merge(heap_block * heap_a, heap_block * heap_b);

/** @brief meld: joining two heaps to form a valid new heap containing all the
 *      elements of both, destroying the original heaps. */
heap_group *
meld(heap_block * heap_a, heap_block * heap_b);

/** TODO: Actually write the mem_alloc function */
void *
mem_alloc(uint16_t obj_size);

#endif // SYSTEM_HEAP_H