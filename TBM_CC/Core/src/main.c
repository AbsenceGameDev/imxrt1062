//#include "../include/gpio_handler.h"
//#include "../include/system_memory_map.h"
#include "gpio_handler.h"
#include "system_memory_map.h"
#include "timer_manager.h"

int
main()
{
  init_gptman_arr(); // zero-init global gpt_manager struct array

  uint32_t seconds = 0x1;
  blinky_led_example(seconds);
  // blinky_led_abstracted_example();
  // blinky_led_original_example();

  // __init_ram_heap__();
  // if (heapb_current->id_n_freed > 1 || heapg_current->_blocks > 1) {
  // }
  return 0;
}
