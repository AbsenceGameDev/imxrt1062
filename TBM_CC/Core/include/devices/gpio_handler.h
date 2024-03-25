/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2024, MIT-License included in project toplevel dir
 */

#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

#include "iomux_controller.h"
#include "timer_manager.h"

// ENUMS

/**
 * @enum gpio_registers_e
 *
 */
typedef enum
{
  DR_DATA_REG = 0x0,               /** @brief Access-rights: RW  */
  GDIR_DIR_REG = 0x4,              /** @brief Access-rights: RW  */
  PSR_PAD_STATUS_REG = 0x8,        /** @brief Access-rights: RO  */
  ICR1_INTERRUPT_CONF_REG1 = 0xc,  /** @brief Access-rights: RW  */
  ICR2_INTERRUPT_CONF_REG2 = 0x10, /** @brief Access-rights: RW  */
  IMR_INTERRUPT_MASK_REG = 0x14,   /** @brief Access-rights: RW  */
  ISR_INTERRUPT_STAT_REG = 0x18,   /** @brief Access-rights: W1C */
  EDGE_SEL = 0x1c,                 /** @brief Access-rights: RW  */
  DR_SET = 0x84,                   /** @brief Access-rights: WO  */
  DR_CLEAR = 0x88,                 /** @brief Access-rights: WO  */
  DR_TOGGLE = 0x8c,                /** @brief Access-rights: WO  */
} gpio_registers_e;

/** @brief: Interrupt Configuration Register (ICR) flags signal detection type */
typedef enum
{
  LOW_LVL_SENSITIVITY = 0x0,
  HIGH_LVL_SENSITIVITY = 0x1,
  RISING_EDGE_SENSITIVITY = 0x2,
  FALLING_EDGE_SENSITIVITY = 0x3,
} gpio_icr_e;

/** @brief Gpio Mux Register (MR) flags for interrupt configuration */
typedef enum
{
  EGPIO_INT_DIS = 0x0,
  EGPIO_INT_CLR_STAT = 0x1
} gpio_mr_e;

/**
 * @brief Edge select. When GPIO_EDGE_SEL[n] is set, the GPIO disregards the ICR[n]
 * setting, and detects any edge on the corresponding input signal.
 **/
typedef enum
{
  EGPIO_EDGE_SET = 0x0,
  EGPIO_EDGE_CLR = 0x1
} gpio_edgesel_e;

/** @brief Register sizes */
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
typedef struct 
{
  muxdev_s*        base_mux_device;
  vuint32_t*       base_addr;
  uint8_t          bit_id;
  uint8_t          value;
  uint8_t          pin;
  gpio_registers_e offsets;
  gpio_io_e        io_type;
} gpiodev_s; // = {.base_mux_device->mux_mode = ALT5_GPIOx_IOx};

// EXTERNS
extern trigger_gpio_fp_t tgpio_fp;
extern gpiodev_s         current_gpio_devices[9];

#define SET_GPIO_GDIR(gpio_gdir_addr, io_type, direction_bit)     \
  *gpio_gdir_addr |= ((0x1 * io_type) << direction_bit)

#define SET_ICR1(gpio_device, setting)                            \
  *(gpio_device->base_addr + ICR1_INTERRUPT_CONF_REG1) = setting

#define SET_ICR2(gpio_device, setting)                            \
  *(gpio_device->base_addr + ICR2_INTERRUPT_CONF_REG2) = setting

#define READ_GPIO(gpio_base_ptr, gpio_register)                   \
  *((uint32_t *)(((uint8_t *)gpio_base_ptr) + gpio_register))

#define SET_GPIO_REGISTER(gpio_reg_addr, direction_bit)           \
  gpio_reg_addr = (0x1 << direction_bit)

/**
 * @param addr vuint32_t *
 * @param byte uint_fast8_t
 */
#define set_iomuxc_byte(addr, byte) *addr = byte;

/**
 * @param addr (vuint32_t *)
 * @param word (uint_fast16_t)
 */
#define set_iomuxc_word(addr, word) *addr = word;
/**
 * @param addr  (vuint32_t *)
 * @param dword (uint_fast32_t)
 */
#define set_iomuxc_dword(addr, dword) *addr = dword;

/**
 * @param gpr_iomuxc_gpr (vuint32_t *)
 */
#define flip_selected_gpr(gpr_iomuxc_gpr) *gpr_iomuxc_gpr = !(*gpr_iomuxc_gpr);

/**
 * @param gpr_iomuxc_gpr (vuint32_t *)
 * @param set_state       (state_e)
 */
#define set_iomuxc_gpr(gpr_iomuxc_gpr, set_state)                              \
  (*gpr_iomuxc_gpr) = (vuint32_t)(0xffffffff * (set_state));

// FUNCTION DECLARATIONS
/**
 * @brief pointer to be able to source pin addresses to the interface
 * @param gpio_pin_addr const volatile uint32_t pointer to a gpio addr
 * @param pulse const uint8_t that indicates low or high pulse
 */
void
trigger_gpio(const uint8_t gpio_device_id, const unsigned char pulse);

void
init_gpio(gpiodev_s*       gpio_device,
          gpio_registers_e gpio_register,
          muxpad_cr_e      pad_group,
          muxpad_dse_e     dse_opt,
          uint8_t          ctrl_pos);

/**
 * @brief: Set all MUX bits in the correct General Purpose Register (GPR)
 **/
void set_gpr_gdir(gpiodev_s* gpio_device);

/** @brief Initializes the stock LED device and return a heap pointer to it's created object */
gpiodev_s* init_onboard_led();

/** @brief Initializes the stock LED device and return a heap pointer to it's created object */
timer_context_s* generate_led_device_context();

/** 
 * @brief Initializes the PIT timer, the stock LED device and returns the constructed timer manager object 
 * 
 * @param timerdatum Object which holds essential the timer data needed for interpreting values 
 * @param interrupt_callback The callback for an potential interrupt-based tick function, fp has no parameter requirements 
 * @param tick_callback The callback for an potential polling-based tick function, fp requires deltatime
 * @return The new timer manager object which has been constructed on the heap 
 */
inline timer_manager_s*
start_PITx(
  timer_datum_s*        timerdatum,
  timer_manager_cb      interrupt_callback,
  timer_manager_sick_cb tick_callback)
  {
    timer_manager_s* pit_timer = (timer_manager_s*)malloc_(sizeof(timer_manager_s));
    pit_timer->timer_ctx = generate_led_device_context();
    
    init_pitman(pit_timer, timerdatum, PIT_CH1, interrupt_callback, tick_callback);  
    setup_PITx(pit_timer, &GPIO7_DR_TOGGLE, 0x3); // start enabled, on-board led 
   
    return pit_timer;
  }

/** 
 * @brief Initializes the PIT timer and the stock LED device on an already existing timer manager 
 * 
 * @param pit_timer A pointer to an already existing timer manager. 
 * @param timerdatum Object which holds essential the timer data needed for interpreting values 
 * @param interrupt_callback The callback for an potential interrupt-based tick function, fp has no parameter requirements 
 * @param tick_callback The callback for an potential polling-based tick function, fp requires deltatime
 */
void 
start_PIT_tick(
  timer_manager_s*      pit_timer,
  timer_datum_s*        timerdatum,
  timer_manager_cb      interrupt_callback,
  timer_manager_sick_cb tick_callback);  

void 
timer_poll(
  timer_manager_s* pit_mgr, 
  timer_datum_s*   timerdatum);


#endif