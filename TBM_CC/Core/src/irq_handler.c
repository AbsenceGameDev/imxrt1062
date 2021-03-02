#include "irq_handler.h"

void
add_to_irq_vector(irq_num_e irq, void_func function)
{
  __vectors_ram__[irq + 16] = function;
  asm volatile("" : : : "memory");
}

void
remove_from_irq_vector(irq_num_e irq, void_func function)
{
  __vectors_ram__[irq + 0x10] = ((void_func)0);
  asm volatile("" : : : "memory");
}