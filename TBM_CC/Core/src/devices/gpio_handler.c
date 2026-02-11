/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2026, MIT-License included in project toplevel dir
 */

#include "gpio_handler.h"

#include "sys/registers.h"

/**
 * @brief TODO GPIO_HANDLER
 * 1. Only use ID's in span [0,8]
 *  2. Keep an array of uninitialized devices, current_gpio_devices[9] and then
 *     keep the corresponding pin to the corresponding index.
 *  3. Whenever an interface needs to trigger gpio, then use aforementioned
 *     device-ID and access current_gpio_devices[ID] to do the stuff needed
 **/

// Function pointer to send through some interfaces,
// for now it only regards display driver interface
trigger_gpio_fp_t tgpio_fp = trigger_gpio;
gpiodev_s current_gpio_devices[9]; // Make addDevice/removeDevice functions

void
trigger_gpio(const uint8_t gpio_device_id, const unsigned char pulse)
{

  // Uncomment below codeblock after I have written add_device and rem_device;
  // current_gpio_devices[gpio_device_id] device;
  // uint8_t dir = device.base_mux_device->ctrl_pos;
  // set_gpr_gdir()
  // set_gpio_datar(&GPIO7_DR_SET, dir);
  // while (some_condition) { // Select GPIOxx macros
  //    if (pulse){ // conditions might need to be flipped for some devices
  //       // Set PIN 13 LOW
  //       clr_gpio_datar(&GPIOxx_DR_CLEAR, dir);
  //    } else {
  //       // Set PIN 13 HIGH,
  //       set_gpio_datar(&GPIOxx_DR_SET, dir);
  //    }
  // }
}

/**
 *
 * High-speed GPIOs exist in this device:
 * •  GPIO 1-5 are standard-speed GPIOs that run off the IPG_CLK_ROOT,
 *    while GPIO 6-9 are high-speed GPIOs that run at the AHB_CLK_ROOT freq.
 *    See the table "System Clocks, Gating, and Override" in CCM chapter.
 *
 * •  Regular GPIO and high speed GPIO are paired
 *    (GPIO1 and GPIO6 share the same pins, GPIO2 and GPIO7 share, etc).
 *    The IOMUXC_GPR_GPR26-29 registers are used to determine if the regular or
 *    high-speed GPIO module is used for the GPIO pins on a given port.
 *
 * Maybe change name of pad_position to control_position?
 *
 * NOTE: The programming sequence for driving output signals should be:
 *
 * 1.  Configure IOMUX to select GPIO mode (Via IOMUXC), also enable SION if
 *     need to read loopback pad value through PSR
 * 2.  Configure GPIO direction register to output (GPIO_GDIR[GDIR] set to 1b).
 * 3.  Write value to data register (GPIO_DR).
 *
 **/
void
init_gpio(gpiodev_s*       gpio_device,
          gpio_registers_e gpio_register,
          muxpad_cr_e      pad_group,
          muxpad_dse_e     dse_opt,
          uint8_t          ctrl_pos)
{
  if (gpio_device == NULL) {
    return;
  }
  switch (pad_group) {
    case GPIO_AD_B0:
      /** @brief: Set MUXmode, ALT5 = GPIO1_IOx x: [0,15] (ctrl_postion + 1) */
      init_device_muxmode(gpio_device->base_mux_device,
                          &IOMUXC_MUX_PAD_GPIO_AD_B0_CR00,
                          &IOMUXC_PAD_PAD_GPIO_AD_B0_CR00,
                          dse_opt,
                          ctrl_pos,
                          MM_ALT5);
      break;
    case GPIO_AD_B1:
      /** @brief: Set MUXmode, ALT5 = GPIO1_IOx x: [16,31] (ctrl_postion + 1) */
      init_device_muxmode(gpio_device->base_mux_device,
                          &IOMUXC_MUX_PAD_GPIO_AD_B1_CR00,
                          &IOMUXC_PAD_PAD_GPIO_AD_B1_CR00,
                          dse_opt,
                          ctrl_pos,
                          MM_ALT5);
      break;
    case GPIO_B0:
      /** @brief: Set MUXmode, ALT5 = GPIO2_IOx x: [0,15] (ctrl_postion + 1) */
      init_device_muxmode(gpio_device->base_mux_device,
                          &IOMUXC_MUX_PAD_GPIO_B0_CR00,
                          &IOMUXC_PAD_PAD_GPIO_B0_CR00,
                          dse_opt,
                          ctrl_pos,
                          MM_ALT5);
      break;
    case GPIO_B1:
      /** @brief: Set MUXmode, ALT5 = GPIO2_IOx x: [16,31] (ctrl_postion + 1) */
      init_device_muxmode(gpio_device->base_mux_device,
                          &IOMUXC_MUX_PAD_GPIO_B1_CR00,
                          &IOMUXC_PAD_PAD_GPIO_B1_CR00,
                          dse_opt,
                          ctrl_pos,
                          MM_ALT5);
      break;
    case GPIO_SD_B0:
      /** @brief: Set MUXmode, ALT5 = GPIO1_IOx x: [12,17] (ctrl_position + 1) */
      init_device_muxmode(gpio_device->base_mux_device,
                          &IOMUXC_MUX_PAD_GPIO_SD_B0_CR00,
                          &IOMUXC_PAD_PAD_GPIO_SD_B0_CR00,
                          dse_opt,
                          ctrl_pos,
                          MM_ALT5);
      break;
    case GPIO_SD_B1:
      /** @brief: Set MUXmode, ALT5 = GPIO1_IOx x: [0,11] (ctrl_position + 1) */
      init_device_muxmode(gpio_device->base_mux_device,
                          &IOMUXC_MUX_PAD_GPIO_SD_B1_CR00,
                          &IOMUXC_PAD_PAD_GPIO_SD_B1_CR00,
                          dse_opt,
                          ctrl_pos,
                          MM_ALT5);
      break;
    default: break;
  }

  gpio_device->base_mux_device->ctrl_pos = ctrl_pos;
  set_gpr_gdir(gpio_device);
}

/**
 * @brief: Set all MUX bits in the correct General Purpose Register (GPR)
 **/
void
set_gpr_gdir(gpiodev_s* gpio_device)
{
  uint8_t     LOW_HIGH = 0x0; // 0001 low, 0000 high
  vuint32_t * gdir_addr = ((vuint32_t*)0);
  switch (gpio_device->pin) {
      // GPR26 [GPIO1,GPIO6]
    case 0x1:
    case 0x6:
      //  Set all MUX bits to either GPIO1 or GPIO6
      IOMUXC_GPR_GPR26 = 0xffffffff * (gpio_device->pin == 0x6);
      break;
      // GPR27 [GPIO2,GPIO7]
    case 0x2:
    case 0x7:
      // Set all MUX bits to either GPIO2 or GPIO7
      IOMUXC_GPR_GPR27 = 0xffffffff * (gpio_device->pin == 0x7);
      break;
      // GPR28 [GPIO3,GPIO8]
    case 0x3:
    case 0x8:
      // Set all MUX bits to either GPIO3 or GPIO8
      IOMUXC_GPR_GPR28 = 0xffffffff * (gpio_device->pin == 0x8);
      break;
      // GPR29 [GPIO4,GPIO9]
    case 0x4:
    case 0x9:
      // Set all MUX bits to either GPIO4 or GPIO9
      IOMUXC_GPR_GPR29 = 0xffffffff * (gpio_device->pin == 0x9);
      break;
  }

  // Setting gpio direction for either output or input
  vuint32_t* redirector = &GPIO1_DIRR;
  switch (gpio_device->pin) {
    case 0x1: redirector = &GPIO1_DIRR; break;
    case 0x2: redirector = &GPIO2_DIRR; break;
    case 0x3: redirector = &GPIO3_DIRR; break;
    case 0x4: redirector = &GPIO4_DIRR; break;
    case 0x5: redirector = &GPIO5_DIRR; break;
    case 0x6: redirector = &GPIO6_DIRR; break;
    case 0x7: redirector = &GPIO7_DIRR; break;
    case 0x8: redirector = &GPIO8_DIRR; break;
    case 0x9: redirector = &GPIO9_DIRR; break;
  }
  SET_GPIO_GDIR(
      redirector, gpio_device->io_type, gpio_device->base_mux_device->ctrl_pos);
}

/**
 * @brief Callback to toggle LED at pin13
 * @note Callback should produce instruction count > 1, meaning it will offset
 * accuracy by atleast one core-cycle each callback */
void
callback_toggle_led()
{
  SET_GPIO_REGISTER(GPIO7_DR_TOGGLE, 0x3);
  PIT_TFLG0_CLR;
  __asm__ volatile("dsb");
}

gpiodev_s*
init_onboard_led()
{
  // inits mux with alt5, and sets pad at DSE 0x7, in pad-group B0 at control
  // register position 3, then sets GPR27 to control, due to pin being 0x7, and
  // will set it to input based on the io_type member
  gpiodev_s* heap_gpio_device = (gpiodev_s*)malloc_(sizeof(gpiodev_s));

  heap_gpio_device->pin = 0x7;
  heap_gpio_device->io_type = GDIR_OUT;
  init_gpio(heap_gpio_device, GDIR_DIR_REG, GPIO_B0, PAD_DSE_R07, 0x3);

  return heap_gpio_device;
}

// @todo
// void SetPrescaler(clkroot) 
// {
// }

timer_context_s* generate_led_device_context()
{
  pit_context_s*   pit_context    = (pit_context_s*)malloc_(sizeof(pit_context_s));
  timer_context_s* timer_context  = (timer_context_s*)malloc_(sizeof(timer_context_s)); 
  timer_context->context          = (void*)pit_context;
  timer_context->base_gpio_device = (void*)init_onboard_led(); // mallocs a device object and sets up some registers for the LED
  timer_context->type = PIT_E; 

  return timer_context;
}

// Polling callback
void timer_poll(timer_manager_s* pit_mgr, timer_datum_s* timerdatum)
{
  // Our range is between 0 to 100 remaining pollings for a triggering the tick callback   
  if (PIT_CVAL0 < 100) 
  {
    float fDeltaSeconds = resolve_time_as_float(timerdatum->freq, MILLIS_E, timerdatum->val - PIT_CVAL0, FROMTICKS_E);
    pit_mgr->tick_callback(fDeltaSeconds);
  }
}