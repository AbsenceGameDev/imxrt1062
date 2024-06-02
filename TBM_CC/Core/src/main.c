/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2024, MIT-License included in project toplevel dir
 */

#include "gpio_handler.h"
#include "sys/memory_map.h"
#include "timer_manager.h"

#include "containers/trb_tree.h"
#include "mocktests_trb_tree.h"


extern inline timer_manager_s*
start_PITx(
  timer_datum_s*        timerdatum,
  timer_manager_cb      interrupt_callback,
  timer_manager_sick_cb tick_callback);

static const timer_manager_s* g_timer_manager = NULL;
static int g_timer_toggle = 1;

// @todo Write a structure to handle signal scheduling, keep this here in main.c as a reminder 
struct { float dummy; } timerschedule_s;

int execute()
{
  test_simplecreation_trb_tree(); // Test that allocating some trees / maps does not crash the teensy


  timer_datum_s     timerdatum = generate_time_struct(PIT_SPEED_50MHz, SECONDS_E, 1);
  timer_manager_s*  pit_timer  = start_PITx(&timerdatum, &hwtick, NULL /* &polltick */); 
  g_timer_manager = pit_timer;

  /** @note @ArioA This calls a polling timer that runs concurrently as the interrupt timers. Interrupt timers take precedent */ 
  while (pit_timer->keep_ticking) 
  {
    if (pit_timer->tick_callback == NULL) { continue; }
    
    timer_poll(pit_timer, &timerdatum);
  }

  g_timer_manager = NULL;

  return 0;
}

// Interrupt/hardware trigger tick and software polling timer tick
void hwtick()
{
  if (g_timer_manager == NULL || g_timer_manager->timer_ctx == NULL) { return; }

  // Set PIN 13 LOW (GPIO7_DR_CLEAR), // Set PIN 13 HIGH (GPIO7_DR_TOGGLE), 
  uint8_t ctrl_pos = ((gpiodev_s*)g_timer_manager->timer_ctx->base_gpio_device)->base_mux_device->ctrl_pos = 0x3;

  switch (g_timer_toggle)
  {
    case 0:
      SET_GPIO_REGISTER(GPIO7_DR_TOGGLE, ctrl_pos);
      g_timer_toggle = 1;
      break;
    case 1:
      SET_GPIO_REGISTER(GPIO7_DR_CLEAR, ctrl_pos);
      g_timer_toggle = 0;
      break;
  }

  PIT_TCTRL0 = 3; 
  PIT_TFLG0_CLR;
  NVIC_ENABLE_IRQ(IRQ_PIT);
  __asm__ volatile("dsb");
}

void polltick(const float delta_time)
{
  hwtick();
}
