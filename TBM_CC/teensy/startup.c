// #include "irq_handler.h"
// #include "mpu.h"
#include "system_heap.h"
#include "system_memory_map.h"

extern unsigned long _stextload;
extern unsigned long _stext;
extern unsigned long _etext;
extern unsigned long _sdataload;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;
extern unsigned long _flexram_bank_config;
extern unsigned long _estack;

#ifndef NVIC_IRQs
  #define NVIC_IRQs 0xa0
#endif

typedef void (*void_func)(void);

static void
memory_copy(uint32_t * dest, const uint32_t * src, uint32_t * dest_end);
static void
memory_clear(uint32_t * dest, uint32_t * dest_end);

void
main();

/**/
__attribute__((/* section(".vectors"),*/ used, aligned(0x400)))
/**
 * @brief IRQ Function vector
 *
 * @attribute: used, aligned(0x400),
 */
void (*volatile __vectors_ram__[NVIC_IRQs + 0x10])(void);

/**/
__attribute__((section(".startup"),
               optimize("no-tree-loop-distribute-patterns"),
               naked))
/**
 * @brief Startup function, program starts here
 *
 * @attribute: sect(".startup"), opt("no-tree-loop-distribute-patterns"), naked,
 */
void
startup()
{
  // FlexRAM bank configuration
  IOMUXC_GPR_GPR17 = (uint32_t)&_flexram_bank_config;
  IOMUXC_GPR_GPR16 = 0x00000007;
  IOMUXC_GPR_GPR14 = 0x00AA0000;
  __asm__ volatile("mov sp, %0" : : "r"((uint32_t)&_estack) :);

  // Initialize memory
  memory_copy(&_stext, &_stextload, &_etext);
  memory_copy(&_sdata, &_sdataload, &_edata);
  memory_clear(&_sbss, &_ebss);

  // enable FPU
  // SCB_CPACR = 0x00F00000;
  __asm__ volatile("CPSIE i" ::: "memory"); // enable irqs

  // Need to read MPU section of ARM refman again to learn why the current
  // implementation of configure_mpu hardfaults the teensy
  // configure_mpu();
  __init_ram_heap__();

  // Call the `main()` function defined in `main.c`.
  main();
}

/**/
__attribute__((section(".startup"),
               optimize("no-tree-loop-distribute-patterns")))
/**
 * @brief
 *
 * @param dest
 * @param src
 * @param dest_end
 * @attribute: sect(".startup"), opt("no-tree-loop-distribute-patterns"),
 */
static void
memory_copy(uint32_t * dest, const uint32_t * src, uint32_t * dest_end)
{
  if (dest == src)
    return;
  while (dest < dest_end) {
    *dest++ = *src++;
  }
}

/**/
__attribute__((section(".startup"),
               optimize("no-tree-loop-distribute-patterns")))
/**
 * @brief
 *
 * @param dest
 * @param dest_end
 * @attribute: sect(".startup"), opt("no-tree-loop-distribute-patterns"),
 */
static void
memory_clear(uint32_t * dest, uint32_t * dest_end)
{
  while (dest < dest_end) {
    *dest++ = 0;
  }
}

/**/
__attribute__((weak))
/**
 * @brief Set as startup NULL if we don't have external memory
 *
 * @attribute: ((weak)),
 */
uint8_t * extmem_next_free = NULL;

/**/
__attribute__((weak))
/**
 * @brief Construct a new malloc extmem object
 *
 * @param cb_alloc
 * @attribute: armgcc - ((weak)),
 */
uint8_t *
malloc_extmem(unsigned int cb_alloc)
{
  if (!extmem_next_free)
    return 0;
  // lets allocate and return on 16 byte boundaries.
  uint8_t * ret_val =
      (uint8_t *)((uint32_t)(extmem_next_free - cb_alloc) & 0xfffffff0);

  // BUGBUG only comparing that we run of EXTERNAL memory not that we overrun
  // pre allocated
  if (ret_val < ((uint8_t *)0x70000000l))
    return 0;
  extmem_next_free = ret_val;
  return ret_val;
}

/**/
__attribute__((weak))
/**
 * @brief
 *
 * @param p
 * @attribute: ((weak)),
 */
void
free_extmem(char * p)
{
  // we don't do any free yet
}
