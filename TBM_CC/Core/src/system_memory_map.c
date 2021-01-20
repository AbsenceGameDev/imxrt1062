#include "system_memory_map.h"

void *
mem_alloc(uint16_t obj_size)
{
  if (obj_size < 1 || (free_stack_ptr + (void *)obj_size) > MEM_END)
    return (void *)0;
  free_stack_ptr += (void *)obj_size;
  return (void *)(free_stack_ptr - (void *)obj_size);
}
