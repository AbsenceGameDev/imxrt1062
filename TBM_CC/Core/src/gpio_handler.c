//#include "../include/gpio_handler.h"
//#include "../include/registers.h"

#include "gpio_handler.h"

#include "registers.h"

#define __disable_irq() __asm__ volatile("CPSID i" ::: "memory");
#define __enable_irq() __asm__ volatile("CPSIE i" ::: "memory");

#define NVIC_ENABLE_IRQ(n)                                                     \
  (*((vuint32_t *)0xE000E100 + ((n) >> 5)) = (1 << ((n)&31)))
#define NVIC_DISABLE_IRQ(n)                                                    \
  (*((vuint32_t *)0xE000E180 + ((n) >> 5)) = (1 << ((n)&31)))

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
  /**
   * current_gpio_devices[gpio_device_id] device;
   * uint8_t dir = device.base_mux_device->ctrl_pos;
   * set_gpr_gdir()
   * set_gpio_datar(&GPIO7_DR_SET, dir);
   * while (some_condition) { // Select correct GPIOxx macros, not sure how yet
   *    if (pulse){ // conditions might need to be flipped for some devices
   *       // Set PIN 13 LOW
   *       clr_gpio_datar(&GPIOxx_DR_CLEAR, dir);
   *    } else {
   *       // Set PIN 13 HIGH,
   *       set_gpio_datar(&GPIOxx_DR_SET, dir);
   *    }
   * }
   *
   **/
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
 *    The IOMUXC_GPR_GPR26-29 registers areused to determine if the regular or
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
init_gpio(gpiodev_s *      gpio_device,
          gpio_registers_e gpio_register,
          muxpad_cr_e      pad_group,
          muxpad_dse_e     dse_opt,
          uint8_t          ctrl_pos)
{
  switch (pad_group) {
    case GPIO_AD_B0:
      /** @brief: Set MUXmode, ALT5 = GPIO1_IOx x: [0,15] (ctrl_postion + 1) */
      init_device_muxmode(gpio_device->base_mux_device,
                          &IOMUXC_MUX_PAD_GPIO_AD_B0_CR00,
                          &IOMUXC_PAD_PAD_GPIO_AD_B0_CR00,
                          dse_opt,
                          ctrl_pos,
                          ALT5_GPIOx_IOx);
      break;
    case GPIO_AD_B1:
      /** @brief: Set MUXmode, ALT5 = GPIO1_IOx x: [16,31] (ctrl_postion + 1) */
      init_device_muxmode(gpio_device->base_mux_device,
                          &IOMUXC_MUX_PAD_GPIO_AD_B1_CR00,
                          &IOMUXC_PAD_PAD_GPIO_AD_B1_CR00,
                          dse_opt,
                          ctrl_pos,
                          ALT5_GPIOx_IOx);
      break;
    case GPIO_B0:
      /** @brief: Set MUXmode, ALT5 = GPIO2_IOx x: [0,15] (ctrl_postion + 1) */
      init_device_muxmode(gpio_device->base_mux_device,
                          &IOMUXC_MUX_PAD_GPIO_B0_CR00,
                          &IOMUXC_PAD_PAD_GPIO_B0_CR00,
                          dse_opt,
                          ctrl_pos,
                          ALT5_GPIOx_IOx);
      break;
    case GPIO_B1:
      /** @brief: Set MUXmode, ALT5 = GPIO2_IOx x: [16,31] (ctrl_postion + 1) */
      init_device_muxmode(gpio_device->base_mux_device,
                          &IOMUXC_MUX_PAD_GPIO_B1_CR00,
                          &IOMUXC_PAD_PAD_GPIO_B1_CR00,
                          dse_opt,
                          ctrl_pos,
                          ALT5_GPIOx_IOx);
      break;
    case GPIO_SD_B0:
      /** @brief: Set MUXmode, ALT5 = GPIO1_IOx x: [12,17] (ctrl_position + 1) */
      init_device_muxmode(gpio_device->base_mux_device,
                          &IOMUXC_MUX_PAD_GPIO_SD_B0_CR00,
                          &IOMUXC_PAD_PAD_GPIO_SD_B0_CR00,
                          dse_opt,
                          ctrl_pos,
                          ALT5_GPIOx_IOx);
      break;
    case GPIO_SD_B1:
      /** @brief: Set MUXmode, ALT5 = GPIO1_IOx x: [0,11] (ctrl_position + 1) */
      init_device_muxmode(gpio_device->base_mux_device,
                          &IOMUXC_MUX_PAD_GPIO_SD_B1_CR00,
                          &IOMUXC_PAD_PAD_GPIO_SD_B1_CR00,
                          dse_opt,
                          ctrl_pos,
                          ALT5_GPIOx_IOx);
      break;
    default: break;
  }
  set_gpr_gdir(gpio_device);

  // Setting gpio direction for either output or input
  vuint32_t * redirector = &GPIO1_DIRR;
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
  (*redirector) |= ((0x1 * gpio_device->io_type) << ctrl_pos);
  uint_fast8_t dir = 0x3;
};

/**
 * @brief: Set all MUX bits in the correct General Purpose Register (GPR)
 **/
void
set_gpr_gdir(gpiodev_s * gpio_device)
{
  uint8_t     LOW_HIGH = 0x0; // 0001 low, 0000 high
  vuint32_t * gdir_addr = ((vuint32_t *)0);
  switch (gpio_device->pin) {
      // GPR26 [GPIO1,GPIO6]
    case 0x1: LOW_HIGH |= 0x1;
    case 0x6:
      //  Set all MUX bits to either GPIO1 or GPIO6
      IOMUXC_GPR_GPR26 = 0xffffffff * (gpio_device->pin == 0x6);
      if (LOW_HIGH == 0x1) {
        gdir_addr = &GPIO1_DIRR;
      } else {
        gdir_addr = &GPIO6_DIRR;
      }
      set_gpio_gdir(gdir_addr,
                    gpio_device->io_type,
                    gpio_device->base_mux_device->ctrl_pos);
      break;
      // GPR27 [GPIO2,GPIO7]
    case 0x2: LOW_HIGH |= 0x1;
    case 0x7:
      // Set all MUX bits to either GPIO2 or GPIO7
      IOMUXC_GPR_GPR27 = 0xffffffff * (gpio_device->pin == 0x7);
      if (LOW_HIGH == 0x1) {
        gdir_addr = &GPIO2_DIRR;
      } else {
        gdir_addr = &GPIO7_DIRR;
      }
      set_gpio_gdir(gdir_addr,
                    gpio_device->io_type,
                    gpio_device->base_mux_device->ctrl_pos);
      break;
      // GPR28 [GPIO3,GPIO8]
    case 0x3: LOW_HIGH |= 0x1;
    case 0x8:
      // Set all MUX bits to either GPIO3 or GPIO8
      IOMUXC_GPR_GPR28 = 0xffffffff * (gpio_device->pin == 0x8);
      if (LOW_HIGH == 0x1) {
        gdir_addr = &GPIO3_DIRR;
      } else {
        gdir_addr = &GPIO8_DIRR;
      }
      set_gpio_gdir(gdir_addr,
                    gpio_device->io_type,
                    gpio_device->base_mux_device->ctrl_pos);
      break;
      // GPR29 [GPIO4,GPIO9]
    case 0x4: LOW_HIGH |= 0x1;
    case 0x9:
      // Set all MUX bits to either GPIO4 or GPIO9
      IOMUXC_GPR_GPR29 = 0xffffffff * (gpio_device->pin == 0x9);
      if (LOW_HIGH == 0x1) {
        gdir_addr = &GPIO4_DIRR;
      } else {
        gdir_addr = &GPIO9_DIRR;
      }
      set_gpio_gdir(gdir_addr,
                    gpio_device->io_type,
                    gpio_device->base_mux_device->ctrl_pos);
      break;
  }
}

void
set_gpio_gdir(vuint32_t * gpio_gdir_addr,
              gpio_io_e   io_type,
              uint8_t     direction_bit)
{
  *gpio_gdir_addr |= ((0x1 * io_type) << direction_bit);
};

uint8_t
handle_gpio(gpiodev_s * gpio_device, gpio_registers_e gpio_register)
{
  vuint32_t * gpio_ptr = (gpio_device->base_addr + gpio_register);
  switch (gpio_register) {

    case GDIR_DIR_REG:
      *gpio_ptr |= (0x1 << gpio_device->bit_id); // Set as OUTPUT
      return 0;
    case PSR_PAD_STATUS_REG:
      return 0x3 | ((*gpio_ptr) >> gpio_device->bit_id); // Read Only
    case DR_SET: // WO
    case DR_CLEAR: // WO
    case DR_TOGGLE: // WO
      *gpio_ptr = (0x1 << gpio_device->bit_id);
      return 0;
    case ICR1_INTERRUPT_CONF_REG1: // regards GPIO [0,15]
      *gpio_ptr = (0x1 << gpio_device->bit_id);
      return 0;
    case ICR2_INTERRUPT_CONF_REG2: // regards GPIO [16,31]
      *gpio_ptr = (0x1 << gpio_device->bit_id);
      return 0;
    case IMR_INTERRUPT_MASK_REG: return 0;
    case ISR_INTERRUPT_STAT_REG: return 0;
    default: return 0;
  }
  return 0;
};

void
set_icr1(gpiodev_s * gpio_device, gpio_icr_e setting)
{
  *(gpio_device->base_addr + ICR1_INTERRUPT_CONF_REG1) = setting;
};

void
set_icr2(gpiodev_s * gpio_device, gpio_icr_e setting)
{
  *(gpio_device->base_addr + ICR2_INTERRUPT_CONF_REG2) = setting;
};

// gpio_icr_e

uint32_t
read_gpio(vuint32_t * gpio_base_ptr, gpio_registers_e gpio_register)
{
  return *((uint32_t *)(((uint8_t *)gpio_base_ptr) + gpio_register));
};

void
set_gpio_register(vuint32_t * gpio_reg_addr, uint_fast8_t direction_bit)
{
  *gpio_reg_addr = (0x1 << direction_bit);
}

void
set_iomuxc_byte(vuint32_t * addr, uint_fast8_t byte)
{
  *addr = byte;
}
void
set_iomuxc_word(vuint32_t * addr, uint_fast16_t word)
{
  *addr = word;
}
void
set_iomuxc_dword(vuint32_t * addr, uint_fast32_t dword)
{
  *addr = dword;
}

void
flip_selected_gpr(vuint32_t * gpr_iomuxc_gpr)
{
  *gpr_iomuxc_gpr = !(*gpr_iomuxc_gpr);
}

void
set_iomuxc_gpr(vuint32_t * gpr_iomuxc_gpr, state_e set_state)
{
  (*gpr_iomuxc_gpr) = (vuint32_t)(0xffffffff * (set_state));
}

// GLOBALS
uint8_t GPT1_CH0_FAUXBOOL = 0x1;

/**
 * @brief Callback to toggle LED at pin13
 * @note Callback should produce two instructions, meaning it will offset
 * accuracy by atleast one core-cycle each callback */
void
callback_pit1_ch1(void)
{
  PIT_TFLG1_CLR;
  // Toggle PIN 13 bewteen HIGH & LOW,
  SET_GPIO_REGISTER(GPIO7_DR_TOGGLE, 0x3);
}

/**
 * @brief: Blinky LED Example
 * Configure GPIO B0_03 (PIN 13) for output, ALT 5 according to p.511 */
void
blinky_led_example(uint32_t seconds, timer_manager_t * pit_mgr)
{
  IOMUXC_MUX_PAD_GPIO_B0_CR03 = 0x5;
  IOMUXC_PAD_PAD_GPIO_B0_CR03 = IOMUXC_PAD_DSE(0x7);
  // GPR27, Set it to Control
  IOMUXC_GPR_GPR27 = 0xffffffff;
  const uint_fast8_t dir = 0x3;
  set_gpio_gdir(&GPIO7_DIRR, GDIR_OUT, dir);

  timer_manager_cb blinker_callback = callback_pit1_ch1;
  timer_s          timer_container;
  init_pitman(pit_mgr,
              PIT_SPEED_50MHz,
              0x0,
              SRC_IPG_CLK,
              YOCTOS_E,
              timer_container,
              0xfffff,
              blinker_callback);

  void * context = pit_mgr->timer_ctx->context;
  pit_mgr->targetval = 0xfffff; // seconds
  CONTEXT_TO_PIT(context)->speedfield = PIT_SPEED_50MHz;
  // setup_pit_timer(&pit_mgr, PIT_TIMER_00);

  CCM_SLCT_PERCLK_SRC(OSC_ROOT);
  CCM_SCMUX1_DIV_SET(0x0);
  CCM_SET_PIT_ENABLE(CLK_ON_ALL_MODES);
  CCM_C_MEOR |= 0x1;

  PIT_MCR_SET(MCR_RESET); // 1.
  PIT_LDVAL1 = 0xff; // ae35f0; // 2.
  PIT_TCTRL1 |= 0x3; // 3. & 4.
  add_to_irq_v(IRQ_PIT, pit_mgr->callback);
  __enable_irq();
  NVIC_ENABLE_IRQ(IRQ_PIT);
  // blinky_led_original_example();
}

// void *
// timer_callback()
// {
// }

/**
 * @brief: Blinky LED Example abstraction (WIP)
 * @TODO: Replace loops with timers. Well, first create some rudimentary timer
 *     functions and then replace loops. Timers chapter at chapter 51. (p.2941)
 **/
void
blinky_led_abstracted_example()
{
  gpiodev_s stack_gpio_device = {.pin = 0x7};
  stack_gpio_device.io_type = GDIR_IN;

  // inits mux with alt5, and sets pad at DSE 0x7, in padgroup B0 at control
  // register position 3, then sets GPR27 to control, du to pin being 0x7, and
  // will set it to input based on the io_type member
  init_gpio(&stack_gpio_device, GDIR_DIR_REG, GPIO_B0, PAD_DSE_R07, 0x3);
  uint8_t dir = stack_gpio_device.base_mux_device->ctrl_pos;
  // Loop the blinky example, if it works it also means malloc works btw, as
  // malloc is used internally in the init_muxmode() function which is called in
  // init_gpio() function
  for (;;) {
    volatile unsigned int i = 0x0;
    volatile unsigned int j = 0x0;

    // Set PIN 13 LOW
    set_gpio_register(&GPIO7_DR_CLEAR, dir);

    // Poor man's delay
    while (i < 0x1ffffff) {
      i++;
      // Poor man's delay
      while (j < 0x1ffffff) {
        j++;
      }
    }
    j = i = 0;

    // Set PIN 13 HIGH,
    set_gpio_register(&GPIO7_DR_SET, dir);

    // Poor man's delay
    while (i < 0x1ffffff) {
      i++;
      // Poor man's delay
      while (j < 0x1ffffff) {
        j++;
      }
    }
    j = i = 0;
  }
}

void
blinky_led_original_example()
{
  IOMUXC_MUX_PAD_GPIO_B0_CR03 = 0x5;
  IOMUXC_PAD_PAD_GPIO_B0_CR03 = IOMUXC_PAD_DSE(0x7);

  // GPR27, Set it to Control
  IOMUXC_GPR_GPR27 = 0xffffffff;
  const uint_fast8_t dir = 0x3;
  set_gpio_gdir(&GPIO7_DIRR, GDIR_OUT, dir);

  void * test = malloc_(0x10); // "malloc"
  if (test == NULL) { // MAKESHIFT DEBUG; BLINK LED TO INDICATE STUFF
    for (;;) {
      volatile unsigned int i = 0x0;
      volatile unsigned int j = 0x0;

      // Set PIN 13 LOW
      set_gpio_register(&GPIO7_DR_CLEAR, dir);

      // Poor man's delay
      while (i < 0x1ffffff) {
        i++;
        // Poor man's delay
        while (j < 0x1ffffff) {
          j++;
        }
      }
      j = i = 0;

      // Set PIN 13 HIGH,
      set_gpio_register(&GPIO7_DR_SET, dir);

      // Poor man's delay
      while (i < 0x1ffffff) {
        i++;
        // Poor man's delay
        while (j < 0x1ffffff) {
          j++;
        }
      }
      j = i = 0;
    }
  }
}