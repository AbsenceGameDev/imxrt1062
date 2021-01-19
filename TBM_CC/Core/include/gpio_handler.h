#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

#include "system_memory_map.h"

EAnalogState
read_state_32t_DataPin(vuint32_t * pin_addr){};

EAnalogState
read_state_16t_DataPin(vuint16_t * pin_addr){};

EAnalogState
read_state_8t_DataPin(vuint8_t * pin_addr){};

typedef enum
{
  DR_DATA_REG = 0x0,
  GDIR_DIR_REG = 0x4,
  PSR_PAD_STATUS_REG = 0x8,
  ICR1_INTERRUPT_CONF_REG1 = 0xc,
  ICR2_INTERRUPT_CONF_REG2 = 0x10,
  IMR_INTERRUPT_MASK_REG = 0x14,
  ISR_INTERRUPT_STAT_REG = 0x18,
  EDGE_SEL = 0x1c,
  DR_SET = 0x84,
  DR_CLEAR = 0x88,
  DR_TOGGLE = 0x8c,
} EBaseGPIO;

typedef enum
{
  BYTE,
  WORD,
  DWORD
} ERegisterSize;

void
handle_gpio(vuint32_t gpio_base_addr,
            EBaseGPIO gpio_register,
            uint8_t   direction_bit){};

uint32_t
read_gpio(vuint32_t gpio_base_addr, EBaseGPIO gpio_register){};
void
set_gpio_muxmode(vuint32_t * gpio_addr, EMuxModes mux_mode){};
void
set_gpio_gdir(vuint32_t * gpio_gdir_addr, uint_fast8_t direction_bit){};
void
set_gpio_datar(vuint32_t * gpio_dr_set_addr, uint_fast8_t direction_bit){};
void
clr_gpio_datar(vuint32_t * gpio_dr_clr, uint_fast8_t direction_bit){};
void
tog_gpio_datar(vuint32_t * gpio_dr_tog, uint_fast8_t direction_bit){};

void
set_iomuxc_byte(vuint32_t * addr, uint_fast8_t byte){};
void
set_iomuxc_word(vuint32_t * addr, uint_fast16_t word){};
void
set_iomuxc_dword(vuint32_t * addr, uint_fast32_t dword){};

void
flip_selected_gpr(vuint32_t gpr_iomuxc_gpr){};
void
set_iomuxc_gpr(vuint32_t gpr_iomuxc_gpr, EState set_state){};

void
blinky_led_example(){};

#endif