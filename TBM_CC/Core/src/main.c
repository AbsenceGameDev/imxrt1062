//#include "../include/gpio_handler.h"
//#include "../include/system_memory_map.h"
#include "gpio_handler.h"
#include "system_memory_map.h"
#include "timer_manager.h"

int
main()
{
  uint32_t        seconds = 0x1;
  pit_context_t   pit_context;
  timer_context_t timer_context;
  timer_context.context = (void *)&pit_context;
  timer_manager_t pit_timer;
  pit_timer.timer_ctx = &timer_context;
  blinky_led_example(seconds, &pit_timer);
  // blinky_led_abstracted_example();
  // blinky_led_original_example();

  // __init_ram_heap__();
  // if (heapb_current->id_n_freed > 1 || heapg_current->_blocks > 1) {
  // }
  return 0;
}
