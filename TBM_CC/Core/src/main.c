//#include "../include/gpio_handler.h"
//#include "../include/system_memory_map.h"
#include "gpio_handler.h"
#include "system_memory_map.h"

int
main()
{
  __init_ram_heap__();
  blinky_led_example();
  // blinky_led_abstracted_example();
  return 0;
}
