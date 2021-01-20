#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

#include "system_memory_map.h"
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
  RO,
  WO,
  RW,
  W1C
} EAccessRights;

typedef enum
{
  LED
} EDeviceType;

typedef enum
{
  SRE = 0x0,
  RSRV0 = 0x2,
  DSE = 0x3,
  SPEED = 0x6,
  RSRV1 = 0x8,
  ODE = 0xb,
  PKE = 0xc,
  PUE = 0xd,
  PUS = 0xe,
  HYS = 0x10,
  RSRV2 = 0x11
} EBitFieldOffset_SWPCPGpio;

typedef enum
{
  ALT0 = 0x0,
  ALT1 = 0x1,
  ALT2 = 0x2,
  ALT3 = 0x3,
  ALT4 = 0x4,
  ALT5 = 0x5
} EMuxMode;

typedef enum
{
  SRE_0_SLOW_SLEW_RATE = 0x0,
  SRE_1_FAST_SLEW_RATE = 0x1
} EBitMuxPad_SRE; // Slew Rate Field

typedef enum
{
  DSE_0_DISABLED = 0x0,
  DSE_1_R0_150O_3_3_V__260O_1_8V = 0x1,
  DSE_2_R0_2 = 0x2,
  DSE_3_R0_3 = 0x3,
  DSE_4_R0_4 = 0x4,
  DSE_5_R0_5 = 0x5,
  DSE_6_R0_6 = 0x6,
  DSE_7_R0_7 = 0x7,
} EBitMuxPad_DSE; // Drive Strength Field

typedef enum
{
  SPEED_0_LOW_50MHz = 0x0,
  SPEED_1_MID_100MHz = 0x1,
  SPEED_2_HI_120MHz = 0x2,
  SPEED_3_MAX_200MHz = 0x3,
} EBitMuxPad_SPEED; // Speed Field

typedef enum
{
  ODE_0_DRAIN_DISABLED = 0x0,
  ODE_1_DRAIN_ENABLED = 0x1
} EBitMuxPad_ODE; // Open Drain Field

typedef enum
{
  PKE_0_KEEPER_DISABLED = 0x0,
  PKE_1_KEEPER_ENABLED = 0x1
} EBitMuxPad_PKE; // Pull/Keep Enable Field

typedef enum
{
  PUE_0_KEEPER = 0x0,
  PUE_1_PULL = 0x1
} EBitMuxPad_PUE; // Pull/Keep Select Field

typedef enum
{
  PUS_0_100K_OHM_PULL_DOWN = 0x0,
  PUS_1_47K_OHM_PULL_UP = 0x1,
  PUS_2_100K_OHM_PULL_UP = 0x2,
  PUS_3_22K_OHM_PULL_UP = 0x3,
} EBitMuxPad_PUS; // Pull Up/Down Config. Field

typedef enum
{
  HYS_0_DISABLED = 0x0,
  HYS_1_ENABLED = 0x1
} EBitMuxPad_HYS; // Hysteresis Enable Field

typedef enum
{
  GPIO_AD_B0,
  GPIO_AD_B1,
  GPIO_B0,
  GPIO_B1,
  GPIO_SD_B0,
  GPIO_SD_B1
} EPadCR;

typedef enum
{
  GDIR_IN,
  GDIR_OUT
} ETypeIO;

typedef union {
  EBitMuxPad_SRE   slew_rate;
  EBitMuxPad_DSE   drive_strength;
  EBitMuxPad_SPEED speed;
  EBitMuxPad_ODE   open_drain;
  EBitMuxPad_PKE   pull_keep_enable;
  EBitMuxPad_PUE   pull_keep_select;
  EBitMuxPad_PUS   pull_up_down_conf;
  EBitMuxPad_HYS   hysteresis_enable;
  EMuxMode         selected_mux_mode;
} UPadFields;

typedef struct {
  vuint32_t * mux_pad_addr;
  vuint32_t * pad_pad_addr;
  UPadFields  current_pad_type;
} SPadContext;

typedef struct {
  uint8_t              bit_id;
  uint8_t              value;
  vuint32_t *          base_addr;
  static const uint8_t pin;
  EBaseGPIO            offsets;
  ETypeIO              io_type;
  uint8_t              ctrl_position;
} SStoredGPIO;

inline void
init_helper(vuint32_t *    SW_MUX,
            vuint32_t *    SW_PAD,
            EBitMuxPad_DSE DSE_OPT,
            uint8_t        control_position,
            ETypeIO        io_type)
{
  *(SW_MUX + control_position) = 0x5;

  /** @brief: Set DSE field (Drive Strength Field) */
  *(SW_PAD + control_position) = IOMUXC_PAD_DSE(DSE_OPT);
}

void
init_gpio(SStoredGPIO    gpio_device,
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
set_gpio_gdir(vuint32_t *  gpio_gdir_addr,
              uint_fast8_t direction_bit,
              ETypeIO      io_type);
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