#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

#include "iomux_controller.h"
#include "timer_manager.h"

// ENUMS
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
} gpio_registers_e;

typedef enum
{
  EGPIO_IN = 0x0,
  EGPIO_OUT = 0x1
} gpio_gdir_e;

typedef enum
{
  LOW_LVL_SENSITIVITY = 0x0,
  HIGH_LVL_SENSITIVITY = 0x1,
  RISING_EDGE_SENSITIVITY = 0x2,
  FALLING_EDGE_SENSITIVITY = 0x3,
} gpio_icr_e;

typedef enum
{
  EGPIO_INT_DIS = 0x0,
  EGPIO_INT_CLR_STAT = 0x1
} gpio_mr_e;

/**
 * Edge select. When GPIO_EDGE_SEL[n] is set, the GPIO disregards the ICR[n]
 * setting, and detects anyedge on the corresponding input signal.
 **/
typedef enum
{
  EGPIO_EDGE_SET = 0x0,
  EGPIO_EDGE_CLR = 0x1
} gpio_edgesel_e;

typedef enum
{
  BYTE,
  WORD,
  DWORD
} regsize_e;

typedef enum
{
  GDIR_IN,
  GDIR_OUT
} gpio_io_e;

// TYPES
// volatile void *void_loc = (volatile void *)0x12345678;
typedef struct {
  uint8_t          bit_id;
  uint8_t          value;
  vuint32_t *      base_addr;
  const uint8_t    pin;
  gpio_registers_e offsets;
  gpio_io_e        io_type;
  muxdev_s *       base_mux_device;
} gpiodev_s; // = {.base_mux_device->mux_mode = ALT5_GPIOx_IOx};

// EXTERNS
extern trigger_gpio_fp_t tgpio_fp;
extern gpiodev_s         current_gpio_devices[9];

#define SET_GPIO_GDIR(gpio_gdir_addr, io_type, direction_bit)                  \
  *gpio_gdir_addr |= ((0x1 * io_type) << direction_bit)

#define SET_ICR1(gpio_device, setting)                                         \
  *(gpio_device->base_addr + ICR1_INTERRUPT_CONF_REG1) = setting

#define SET_ICR2(gpio_device, setting)                                         \
  *(gpio_device->base_addr + ICR2_INTERRUPT_CONF_REG2) = setting

#define READ_GPIO(gpio_base_ptr, gpio_register)                                \
  *((uint32_t *)(((uint8_t *)gpio_base_ptr) + gpio_register))

#define SET_GPIO_REGISTER(gpio_reg_addr, direction_bit)                        \
  gpio_reg_addr = (0x1 << direction_bit)

// FUNCTION DECLARATIONS
/**
 * @brief pointer to be able to source pin addresses to the interface
 * @param gpio_pin_addr const volatile uint32_t pointer to a gpio addr
 * @param pulse const uint8_t that indicates low or high pulse
 */
void
trigger_gpio(const uint8_t gpio_device_id, const unsigned char pulse);

void
init_gpio(gpiodev_s *      gpio_device,
          gpio_registers_e gpio_register,
          muxpad_cr_e      pad_group,
          muxpad_dse_e     dse_opt,
          uint8_t          ctrl_pos);

/**
 * @brief: Set all MUX bits in the correct General Purpose Register (GPR)
 **/
void
set_gpr_gdir(gpiodev_s * gpio_device);

uint8_t
handle_gpio(gpiodev_s * gpio_device, gpio_registers_e gpio_register);

void
set_icr1(gpiodev_s * gpio_device, gpio_icr_e setting);

void
set_icr2(gpiodev_s * gpio_device, gpio_icr_e setting);

uint32_t
read_gpio(vuint32_t * gpio_base_ptr, gpio_registers_e gpio_register);

void
set_gpio_gdir(vuint32_t * gpio_gdir_addr,
              gpio_io_e   io_type,
              uint8_t     direction_bit);
void
set_gpio_register(vuint32_t * gpio_reg_addr, uint_fast8_t direction_bit);

void
set_iomuxc_byte(vuint32_t * addr, uint_fast8_t byte);
void
set_iomuxc_word(vuint32_t * addr, uint_fast16_t word);
void
set_iomuxc_dword(vuint32_t * addr, uint_fast32_t dword);

void
flip_selected_gpr(vuint32_t * gpr_iomuxc_gpr);
void
set_iomuxc_gpr(vuint32_t * gpr_iomuxc_gpr, state_e set_state);

void
blinky_led_example(uint32_t seconds, timer_manager_t * pit_mgr);

void
blinky_led_abstracted_example();

void
blinky_led_original_example();

#endif