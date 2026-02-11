/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2026, MIT-License included in project toplevel dir
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

static timer_manager_s* g_timer_manager = NULL;
static int g_timer_toggle = 1;

// @todo Write a structure to handle signal scheduling, keep this here in main.c as a reminder 
struct { float dummy; } timerschedule_s;

void tree_tests(bool should_test)
{
  if (should_test)
  {
    test_simplecreation_trb_tree(); // Test of allocating trees with POD data: SUCCESS
    test_simplecreation_trb_tree(); // Test of allocating trees with POD data: SUCCESS
    test_simplecreation_trb_tree(); // Test of allocating trees with POD data: SUCCESS
    test_simplecreation_trb_tree(); // Test of allocating trees with POD data: SUCCESS
    // test_complexcreation_trb_tree(); // Test of allocating trees with non POD data: CRASH 
  }
}

int execute()
{
  tree_tests(false);  

  timer_datum_s     timerdatum = generate_time_struct(PIT_SPEED_200MHz, MILLIS_E, 100);
  timer_manager_s*  pit_timer  = start_PITx(&timerdatum, &hwtick, &polltick); 
  g_timer_manager = pit_timer;

  /** @note @ArioA This calls a polling timer that runs concurrently as the interrupt timers. Interrupt timers take precedent */ 
  for (;pit_timer->keep_ticking;)
  {
    timer_poll(pit_timer, &timerdatum);
  }

  g_timer_manager = NULL;
  return 0;
}

// Interrupt/hardware trigger tick and software polling timer tick
void hwtick()
{
  polltick(0.0); // Interrupt based, will not have or need deltatime

  PIT_TCTRL0 = 3; 
  PIT_TFLG0_CLR;
  NVIC_ENABLE_IRQ(IRQ_PIT);
  __asm__ volatile("dsb");
}

// Tick: Currently only setting PIN 13 LOW (GPIO7_DR_CLEAR), // Setting PIN 13 HIGH (GPIO7_DR_TOGGLE), 
void polltick(const float delta_time)
{
  uint8_t ctrl_pos = (g_timer_manager->timer_ctx != NULL) 
    ? ((gpiodev_s*)g_timer_manager->timer_ctx->base_gpio_device)->base_mux_device->ctrl_pos 
    : 0x3; // Fallback to default LED control position

  led_toggle(delta_time, ctrl_pos);

  // Reset pit timer for PIT_0
  // TODO: Investigate. NOTE: Does not seem to reset the timer!
  PIT_TCTRL0 = 3; 
  PIT_TFLG0_CLR;
  setup_PITx(g_timer_manager);
}


void led_toggle(const float delta_time, vuint32_t ctrl_pos)
{
  switch (g_timer_toggle)
  {
    case 0:
      g_timer_toggle = 1;
      SET_GPIO_REGISTER(GPIO7_DR_SET, ctrl_pos);
      break;
    case 1:
      g_timer_toggle = 0;
      SET_GPIO_REGISTER(GPIO7_DR_CLEAR, ctrl_pos);
      break;
    default: 
      g_timer_toggle = 0;
    break;
  }
}