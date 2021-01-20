#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

#include "iomux_controller.h"

typedef enum
{
  DR_DATA_REG = 0x0, // RW
  GDIR_DIR_REG = 0x4, // RW
  PSR_PAD_STATUS_REG = 0x8, // RO
  ICR1_INTERRUPT_CONF_REG1 = 0xc, // RW
  ICR2_INTERRUPT_CONF_REG2 = 0x10, // RW
  IMR_INTERRUPT_MASK_REG = 0x14, // RW
  ISR_INTERRUPT_STAT_REG = 0x18, // W1C
  EDGE_SEL = 0x1c, // RW
  DR_SET = 0x84, // WO
  DR_CLEAR = 0x88, // WO
  DR_TOGGLE = 0x8c, // WO
} EBaseGPIO;

typedef enum
{
  EGPIO_IN = 0x0,
  EGPIO_OUT = 0x1
} EGpioGDir;

typedef enum
{
  LOW_LVL_SENSITIVITY = 0x0,
  HIGH_LVL_SENSITIVITY = 0x1,
  RISING_EDGE_SENSITIVITY = 0x2,
  FALLING_EDGE_SENSITIVITY = 0x3,
} E_ICRFIELDS_GPIO;

typedef enum
{
  EGPIO_INT_DIS = 0x0,
  EGPIO_INT_CLR_STAT = 0x1
} EGpioIMR;

/**
 * Edge select. When GPIO_EDGE_SEL[n] is set, the GPIO disregards the ICR[n]
 * setting, and detects anyedge on the corresponding input signal.
 **/
typedef enum
{
  EGPIO_EDGE_SET = 0x0,
  EGPIO_EDGE_CLR = 0x1
} EGpioEdgeSelect;

typedef enum
{
  BYTE,
  WORD,
  DWORD
} ERegisterSize;

typedef enum
{
  GDIR_IN,
  GDIR_OUT
} ETypeIO;

// volatile unsigned int *myPointer = (volatile unsigned int *)0x12345678;
typedef struct {
  uint8_t              bit_id;
  uint8_t              value;
  vuint32_t *          base_addr;
  static const uint8_t pin;
  EBaseGPIO            offsets;
  ETypeIO              io_type;
  SStoredMUXDevice *   base_mux_device;
} SStoredGPIO = {.base_mux_device.mux_mode = ALT5_GPIOx_IOx};

void
init_gpio(SStoredGPIO *  gpio_device,
          EBaseGPIO      gpio_register,
          EPadCR         pad_group,
          EBitMuxPad_DSE DSE_OPT);

/**
 * @brief: Set all MUX bits in the correct General Purpose Register (GPR)
 **/
void
set_gpr_gdir(SStoredGPIO * gpio_device);

void *
handle_gpio(SStoredGPIO gpio_device, EBaseGPIO gpio_register);

void
set_icr1(SStoredGPIO * gpio_device, E_ICRFIELDS_GPIO setting);

void
set_icr2(SStoredGPIO * gpio_device, E_ICRFIELDS_GPIO setting);

uint32_t
read_gpio(vuint32_t gpio_base_addr, EBaseGPIO gpio_register);
void
set_gpio_muxmode(vuint32_t * gpio_addr, EMuxModes mux_mode);
void
set_gpio_gdir(vuint32_t * gpio_gdir_addr,
              ETypeIO     io_type,
              uint8_t     direction_bit);
void
set_gpio_datar(vuint32_t * gpio_dr_set_addr, uint_fast8_t direction_bit);
void
clr_gpio_datar(vuint32_t * gpio_dr_clr, uint_fast8_t direction_bit);
void
tog_gpio_datar(vuint32_t * gpio_dr_tog, uint_fast8_t direction_bit);

void
set_iomuxc_byte(vuint32_t * addr, uint_fast8_t byte);
void
set_iomuxc_word(vuint32_t * addr, uint_fast16_t word);
void
set_iomuxc_dword(vuint32_t * addr, uint_fast32_t dword);

void
flip_selected_gpr(vuint32_t gpr_iomuxc_gpr);
void
set_iomuxc_gpr(vuint32_t gpr_iomuxc_gpr, EState set_state);

void
blinky_led_example(){};

#endif