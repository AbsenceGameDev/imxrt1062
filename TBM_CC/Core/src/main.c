/**
 * @file      main.c
 * @author    Ario@Permadev
 * @brief
 * @version   0.1
 * @date      2021-08-29
 *
 * @copyright Copyright (c) 2021, MIT-License included in project toplevel dir
 *
 */

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

  blinky_led_example_PIT(seconds, &pit_timer);

  while (1) {
  }

  return 0;
}
