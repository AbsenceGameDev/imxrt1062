//#include "../include/gpio_handler.h"
//#include "../include/system_memory_map.h"
#include "gpio_handler.h"
#include "system_memory_map.h"

int
main()
{
  __init_ram_heap__();
  if (heapb_current->id_n_freed > 1 || heapg_current->_blocks > 1) {
    blinky_led_example();
  }
  // blinky_led_abstracted_example();
  return 0;
}
