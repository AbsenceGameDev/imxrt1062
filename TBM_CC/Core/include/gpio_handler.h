#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

#include "system_memory_map.h"

EAnalogState
read_state_32t_DataPin(vuint32_t * pin_addr){};

EAnalogState
read_state_16t_DataPin(vuint16_t * pin_addr){};

EAnalogState
read_state_8t_DataPin(vuint8_t * pin_addr){};

void
set_gpio_muxmode(vuint32_t * gpio_addr, EMuxModes mux_mode)
{
  *gpio_addr = mux_mode;
}

void
set_gpio_gdir(vuint32_t * gpio_gdir_addr, uint_fast8_t direction_bit)
{
  *gpio_gdir_addr |= (0x1 << direction_bit);
}

void
set_gpio_datar(vuint32_t * gpio_dr_set_addr, uint_fast8_t direction_bit)
{
  *gpio_dr_set_addr |= (0x1 << direction_bit);
}

void
clr_gpio_datar(vuint32_t * gpio_dr_clr, uint_fast8_t direction_bit)
{
  *gpio_dr_clr |= (0x1 << direction_bit);
}

void
tog_gpio_datar(vuint32_t * gpio_dr_tog, uint_fast8_t direction_bit)
{
  *gpio_dr_tog |= (0x1 << direction_bit);
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
flip_selected_gpr(vuint32_t gpr_iomuxc_gpr)
{
  *gpr_iomuxc_gpr = !(gpr_iomuxc_gpr);
}

void
set_iomuxc_gpr(vuint32_t gpr_iomuxc_gpr, EState set_state)
{
  (*gpr_iomuxc_gpr) = 0xffffffff * (set_state);
}

/**
 * @brief: Blinky LED Example
 * Configure GPIO B0_03 (PIN 13) for output, ALT 5 according to p.511 */
void
blinky_led_example()
{
  IOMUXC_MUX_PAD_GPIO_B0_CR03 = 0x5;
  IOMUXC_MUX_PAD_GPIO_B0_CR03 = IOMUXC_PAD_DSE(0x7);

  // GPR27, Set it to Control
  IOMUXC_GPR_GPR27 = 0xffffffff;
  uint_fast8_t dir = 0x3;

  /** GPIO_GDIR functions as direction control when the IOMUXC is in GPIO mode.
   *  Each bit specifies the direction of a one-bit signal.
   **/
  // Set DPIO7 direction (position), in GDIR
  set_gpio_gdir(GPIO7_DIRR, dir);

  for (;;) {
    volatile unsigned int i = 0x0;

    // Set PIN 13 HIGH,
    set_gpio_datar(GPIO7_DR_SET, dir);

    // Poor man's delay
    while (i < 0x10000) {
      i++;
    }

    i = 0;

    // Set PIN 13 LOW
    clr_gpio_datar(GPIO7_DR_CLEAR, dir);

    // Poor man's delay
    while (i < 0x010000) {
      i++;
    }
  }
}

#endif