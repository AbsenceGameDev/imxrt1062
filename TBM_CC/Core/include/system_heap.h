#ifndef SYSTEM_HEAP_H
#define SYSTEM_HEAP_H

#include "system_memory_map.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define NULL_ADDR ((volatile void *)0)

/**
 * @brief: OCRAM TrustZone (TZ) enable macro.
 *
 * 0x0: The TrustZone feature is disabled. Entire OCRAM space is available for
 * all access types (secure/non-secure/user/supervisor).
 *
 * 0x1: The TrustZone feature is enabled. Access to address in the range
 * specified by[ENDADDR : STARTADDR] follows the execution mode access policy
 * described in CSU chapter */
#define SET_OCRAM_TRUSZONE(x) IOMUXC_GPR_GPR10 |= (((x)&0x1) << 8)
#define HG_HEAD_BLOCK(heapg)                                                   \
  (heap_block *)(((vuint8_t *)(heapg)) + HB_HEADER_SIZE)

// OCRAM FLEXRAM (FLEXIBLE MEMORY ARRAY, will use for heap space)
#define MEM_START SYSMEM_DTCM_S // S - 0x00040000)
#define MEM_END SYSMEM_DTCM_E // reserving 128kb for mem_alloc
volatile void * free_heap_ptr = (volatile void *)MEM_START;
#define MEM_OFFS(x) (MEM_START + (x))

// Split the into N amounts of 32KB segments,
// N is based on the totalt sice
#define __gen_heap_group_in_region(start_addr_heap, end_addr_heap)             \
  heapg_head = (heap_group *)(start_addr_heap);                                \
  heap_group * current_heap = (heap_group *)(start_addr_heap);                 \
  heapg_head->next = (heap_group *)(((vuint8_t *)current_heap) + 0x8000);      \
  heapg_head->prev = (heap_group *)NULL;                                       \
  heapg_head->_size = 0x80008000;                                              \
  heapg_head->group_id = 0x0;                                                  \
  heapb_current = HBHG_INCR_ADDR(heapg_head, HG_HEADER_SIZE);                  \
  heapb_current->data_size = MAX_HB_DATA_SIZE;                                 \
  heapb_current->prev = (heap_block *)NULL;                                    \
  heapb_current->next = (heap_block *)NULL;                                    \
  SET_BLOCK_FREE(heapb_current);                                               \
  SET_GROUP_ID(heapb_current->id_n_freed, 0x0);                                \
                                                                               \
  uint32_t KBSize = (((end_addr_heap) - (start_addr_heap)) + 0x3ff) / 0x400;   \
  (--KBSize);                                                                  \
  KBSize /= 32;                                                                \
                                                                               \
  for (uint32_t i = 0; i < KBSize; i++) {                                      \
    current_heap = current_heap->next;                                         \
    current_heap->next = (heap_group *)(((vuint8_t *)current_heap) + 0x8000);  \
    current_heap->prev = (current_heap - 0x8000);                              \
    current_heap->_size = 0x80008000;                                          \
    current_heap->group_id = 0x1 + i;                                          \
    heapb_current = HBHG_INCR_ADDR(current_heap, HG_HEADER_SIZE);              \
    heapb_current->data_size = MAX_HB_DATA_SIZE;                               \
    heapb_current->prev = (heap_block *)NULL;                                  \
    heapb_current->next = (heap_block *)NULL;                                  \
    SET_BLOCK_FREE(heapb_current);                                             \
    SET_GROUP_ID(heapb_current->id_n_freed, 0x1 + i);                          \
  }                                                                            \
  current_heap->next = (heap_group *)NULL

#define __set_designated_heap(s_addr, e_addr, frag_s_addr, frag_e_addr)        \
  designated_heap.start_addr_heap = (volatile void *)(s_addr);                 \
  designated_heap.end_addr_heap = (volatile void *)(e_addr);                   \
  designated_heap.frag_start_addr_heap = (volatile void *)(frag_s_addr);       \
  designated_heap.frag_end_addr_heap = (volatile void *)(frag_e_addr);

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
 * @param FLEXRAM_O50_I50_D00 0xff55ff55 / ITCM 50%, OCRAM 50%, DTCM 00%
 * @param FLEXRAM_O50_I00_D50 0xaa55aa55 / DTCM 50%, OCRAM 50%, ITCM 00%
 * @param FLEXRAM_NO_DEFAULT  0x0        / NO PRESET VALUE
 *
 * @note
 * DTCM - Tightly-Coupled Mem for Data:
 *        For time critical data handling (stack, heap...)
 *
 * ITCM - Tightly-Coupled Mem for Instructions:
 *        For time-critical routines
 **/
typedef enum
{
  FLEXRAM_OCRAM_ONLY = 0x55555555,
  FLEXRAM_O25_I50_D25 = 0xffa5ffa5,
  FLEXRAM_O25_I25_D50 = 0xf5aaf5aa,
  FLEXRAM_O00_I50_D50 = 0xffaaffaa,
  FLEXRAM_O50_I00_D50 = 0x55aa55aa,
  FLEXRAM_O50_I50_D00 = 0x55ff55ff,
  FLEXRAM_O50_I25_D25 = 0x55af55af,
  FLEXRAM_NO_DEFAULT = 0x0
} ERAMBankConf;

typedef struct {
  volatile void * start_addr_heap;
  volatile void * end_addr_heap;
  volatile void * frag_start_addr_heap;
  volatile void * frag_end_addr_heap;
} heap_region;
heap_region designated_heap;

typedef uint8_t heap_group_t;
/**
 * @brief Heap Group struct
 * NOTE: Size of this struct is 17 Bytes (0x11 Bytes)
 * NOTE: Start address of heap group will be the address of a given actual
 * heap_group pointer, 1st heap_block starts offset 0x11 bytes
 *
 * @param prev  Pointer to previous heapgroup, NULL if current is Head.
 * @param next Pointer to next heapgroup, NULL if current is End.
 * @param group_id Integer ID for the heap_group
 * @param _size 32-bit field: [0,15]: Total Size  [16,31]: Free Size
 * @param _blocks 32-bit field:  USED BLOCKS [0,15].  FREE BLOCKS [16,31].
 **/
struct heap_group_s {
  struct heap_group_s * prev; // 4 Bytes
  struct heap_group_s * next; // 4 Bytes
  heap_group_t          group_id; // 1 Bytes
  uint32_t              _size; // 4 Bytes
  uint32_t              _blocks; // 4 Bytes
};
typedef struct heap_group_s heap_group;
#define HG_HEADER_SIZE 0x11

/**
 * @brief Macros for setting/changing vals in the 32-bit fields in heap_group.
 * Overflow Should not be possible, so I will not make edgecases for it,
 * K.I.S.S.
 **/
#define SET_HEAP_TOTAL(sp, val) ((sp) = ((sp) & ~0xffff) | ((val)&0xffff))
#define ADD_HEAP_TOTAL(sp, add)                                                \
  ((sp) = ((sp) & ~0xffff) | (((sp)&0xffff) + (add)))
#define SUB_HEAP_TOTAL(sp, sub)                                                \
  ((sp) = ((sp) & ~0xffff) | (((sp)&0xffff) - (sub)))
#define SET_HEAP_FREE(sp, val) ((sp) = ((sp)&0xffff) | ((val) & ~0xffff))
#define ADD_HEAP_FREE(sp, add)                                                 \
  ((sp) = ((sp)&0xffff) | (((sp) & ~0xffff) + (add)))
#define SUB_HEAP_FREE(sp, sub)                                                 \
  ((sp) = ((sp)&0xffff) | (((sp) & ~0xffff) - (sub)))
#define READ_HEAP_FREE(heap_g) (((heap_g)->_size >> 0x10) & 0x10)
#define READ_HEAP_TOTAL(heap_g) ((heap_g)->_size & 0x10)
#define READ_HEAP_FREEBLOCKS(heap_g) (((heap_g)->_blocks >> 0x10) & 0x10)
#define READ_HEAP_USEDBLOCKS(heap_g) ((heap_g)->_blocks & 0x10)

#define HGHG_INCR_ADDR(heapg, n) (heap_group *)(((vuint8_t *)(heapg)) + (n))

/**
 * @brief Heap Block struct
 * NOTE: Size of this struct is 11 Bytes (0xb Bytes)
 * NOTE: Start address of heap block will be the address of a given actual
 *       heap_block pointer, data of heap_block starts offset 0xd bytes
 *
 * @param prev  Pointer to previous heap_block, NULL if current is Head.
 * @param next Pointer to next heap_block, NULL if current is End.
 * @param data_size uint16_t, size of data block in bytes
 * @param id_n_freed bit 0: Free, bit 1-3: Reserved, bit 4-7: group ID
 *
 **/
struct heap_block_s {
  struct heap_block_s * prev; // 4 Bytes
  struct heap_block_s * next; // 4 Bytes
  uint16_t              data_size; // 2 Bytes
  uint8_t               id_n_freed; // 1 Byte
};
typedef struct heap_block_s heap_block;

#define HB_HEADER_SIZE 0xb
#define READ_BLOCK_FREE(heapblock) ((heapblock)->id_n_freed & 0x1)
#define SET_BLOCK_FREE(heap_b)                                                 \
  (heap_b)->id_n_freed = (((heap_b)->id_n_freed & ~0x1) | 0x1)
#define SET_BLOCK_USED(heap_b)                                                 \
  (heap_b)->id_n_freed = (((heap_b)->id_n_freed & ~0x1) | 0x0)
#define SET_GROUP_ID(field, id) field = ((((id) << 0x4) & ~0x1) | ((field)&0x1))
#define READ_BLOCK_GID(idfreed) (((idfreed) >> 0x4) & 0x4)
#define BLOCK_END(hb_cptr) ((hb_cptr) + (hb_cptr)->data_size + HB_HEADER_SIZE)
#define MAX_HB_DATA_SIZE (0x8000 - HB_HEADER_SIZE - HG_HEADER_SIZE)
#define HBHG_INCR_ADDR(heapb, n) (heap_block *)(((vuint8_t *)(heapb)) + (n))
#define VOID_INCR_ADDR(any_type, n) (void *)(((vuint8_t *)(any_type)) + (n))

uint16_t g_free_blocks[0x10];
uint16_t g_used_blocks[0x10];

heap_group * heapg_head;
heap_group * heapg_current;
heap_block * heapb_current;

typedef enum
{
  FUSE_VAL_CONF,
  GPR17_BANK_CFG
} EFlexSrcSelect;

typedef enum
{
  TCM_DISABLE = 0x0,
  TCM_ENABLE = 0x1
} EInitTCM;

typedef enum
{
  TZ_DISABLE = 0x0,
  TZ_ENABLE = 0x1
} ETrustZoneState;

/**
 * @brief Memory Allocation
 * @param obj_size  Size (in Bytes) of requested object.
 * @return A void* with address of object. NULL if failed.
 **/
void *
malloc_(uint16_t obj_size);

/**
 * @brief Free a pointer
 * calls __remove_block__ and sets the pointer to null
 * @param ptr Pointer to free
 * */
void
free(void * ptr);

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
void
generate_heap_groups(uint16_t heap_byte_size);

/** @brief create-heap: create an empty heap */
heap_group *
create_heap(uint16_t heap_byte_size);

/** @brief heapify: create a heap out of given array of elements */
heap_group *
heapify(void * context);

/** @brief merge (union): joining two heaps to form a valid new heap containing
 * all the elements of both, preserving the original heaps. */
heap_group *
merge(heap_block * heap_ba, heap_block * heap_bb);

/** @brief meld: joining two heaps to form a valid new heap containing all the
 *      elements of both, destroying the original heaps. */
heap_group *
meld(heap_block * heap_ba, heap_block * heap_bb);

void *
__find_mem__(heap_group * heap_g, uint16_t size);

void
__compactation__(heap_group * heap_g);

/**
 * @brief Free Block of memory
 * Clearing data isn't actually needed
 **/
void
__remove_block__(heap_block * heap_b);

/**
 * @brief Coalesce Blocks of memory recursively, first frontwards from the end
 * to given block, then backwards from the given block to the start
 *
 * NOTE: Relinking the coalesced block with new 'prev' and 'next' pointers
 *       happens within the __coalesce__ internal functions __coalesce_front__
 *       and __coalesce_back__
 **/
void
__coalesce__(heap_block * heap_b);

/**
 * @brief Coalesce Blocks of memory recursively, frontwards
 * @param heap_b pointer to block to coalesce forward from
 **/
void
__coalesce_front__(heap_block * heap_b);

/**
 * @brief Coalesce Blocks of memory recursively, backwards
 * @param heap_b pointer to block to coalesce backward from
 * @return Returns a pointer to the 'prev' pointer
 **/
heap_block *
__coalesce_back__(heap_block * heap_b);

void
__data_swap_next__(heap_block * heap_b);

#endif // SYSTEM_HEAP_H