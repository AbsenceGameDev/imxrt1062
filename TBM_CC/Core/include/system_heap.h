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
#define MEM_START SYSMEM_OCRAM_FLEX_S
#define MEM_END SYSMEM_OCRAM_FLEX_E
extern volatile void * free_heap_ptr;
#define MEM_OFFS(x) (MEM_START + (x))

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

typedef struct {
  volatile void * start_addr_heap;
  volatile void * end_addr_heap;
  volatile void * frag_start_addr_heap;
  volatile void * frag_end_addr_heap;
} heap_region;
extern heap_region designated_heap;

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
  volatile struct heap_group_s * prev; // 4 Bytes
  volatile struct heap_group_s * next; // 4 Bytes
  heap_group_t                   group_id; // 1 Bytes
  uint32_t                       _size; // 4 Bytes
  uint32_t                       _blocks; // 4 Bytes
};
typedef struct heap_group_s heap_group;
typedef volatile heap_group vheap_group;
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
#define READ_HEAP_FREEBLOCKS(heap_g) ((heap_g->_blocks >> 0x10) & 0xff)
#define READ_HEAP_USEDBLOCKS(heap_g) ((heap_g)->_blocks & 0x10)

#define HGHG_INCR_ADDR(heapg, n) (vheap_group *)(((vuint8_t *)(heapg)) + (n))

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
  volatile struct heap_block_s * prev; // 4 Bytes
  volatile struct heap_block_s * next; // 4 Bytes
  uint16_t                       data_size; // 2 Bytes
  uint8_t                        id_n_freed; // 1 Byte
};
typedef struct heap_block_s heap_block;
typedef volatile heap_block vheap_block;

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
#define HBHG_INCR_ADDR(heapb, n) (vheap_block *)(((vuint8_t *)(heapb)) + (n))
#define VOID_INCR_ADDR(any_type, n) (void *)(((vuint8_t *)(any_type)) + (n))

extern uint16_t g_free_blocks[0x10];
extern uint16_t g_used_blocks[0x10];

extern vheap_group * heapg_head;
extern vheap_group * heapg_current;
extern vheap_block * heapb_current;

#define __set_designated_heap(s_addr, e_addr, frag_s_addr, frag_e_addr)        \
  designated_heap.start_addr_heap = (volatile void *)(s_addr);                 \
  designated_heap.end_addr_heap = (volatile void *)(e_addr);                   \
  designated_heap.frag_start_addr_heap = (volatile void *)(frag_s_addr);       \
  designated_heap.frag_end_addr_heap = (volatile void *)(frag_e_addr);

typedef enum
{
  FUSE_VAL_CONF,
  GPR17_BANK_CFG
} flex_src_select_e;

typedef enum
{
  TCM_DISABLE = 0x0,
  TCM_ENABLE = 0x1
} init_tcm_e;

typedef enum
{
  TZ_DISABLE = 0x0,
  TZ_ENABLE = 0x1
} trustzone_state_e;

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
free_(void * ptr);

/** @brief Heap creation funcs. */
void
__init_ram_heap__();

void
__gen_single_heapg__(uint32_t start_addr_heap, uint8_t idx);

/** @brief create-heap: create an empty heap */
vheap_group *
create_heap(uint16_t heap_byte_size);

/** @brief heapify: create a heap out of given array of elements */
vheap_group *
heapify(void * context);

/** @brief merge (union): joining two heaps to form a valid new heap containing
 * all the elements of both, preserving the original heaps. */
vheap_group *
merge(vheap_block * heap_ba, heap_block * heap_bb);

/** @brief meld: joining two heaps to form a valid new heap containing all the
 *      elements of both, destroying the original heaps. */
heap_group *
meld(vheap_block * heap_ba, heap_block * heap_bb);

void *
__find_mem__(vheap_group * heap_g, uint16_t size);

void
__compactation__(vheap_group * heap_g);

/**
 * @brief Free Block of memory
 * Clearing data isn't actually needed
 **/
void
__remove_block__(vheap_block * heap_b);

/**
 * @brief Coalesce Blocks of memory recursively, first frontwards from the end
 * to given block, then backwards from the given block to the start
 *
 * NOTE: Relinking the coalesced block with new 'prev' and 'next' pointers
 *       happens within the __coalesce__ internal functions __coalesce_front__
 *       and __coalesce_back__
 **/
void
__coalesce__(vheap_block * heap_b);

/**
 * @brief Coalesce Blocks of memory recursively, frontwards
 * @param heap_b pointer to block to coalesce forward from
 **/
void
__coalesce_front__(vheap_block * heap_b);

/**
 * @brief Coalesce Blocks of memory recursively, backwards
 * @param heap_b pointer to block to coalesce backward from
 * @return Returns a pointer to the 'prev' pointer
 **/
vheap_block *
__coalesce_back__(vheap_block * heap_b);

void
__data_swap_next__(vheap_block * heap_b);

#endif // SYSTEM_HEAP_H