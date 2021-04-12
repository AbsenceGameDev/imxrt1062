#ifndef IOMUX_CONTROLLER_H
#define IOMUX_CONTROLLER_H

#include "system_heap.h"
#include "system_memory_map.h"
#ifndef NULL
#define NULL ((void *)0)
#endif

typedef enum
{
  ALT0_LCD_DATAx = 0x0, // LCD_DATAx
  ALT1_QTIMERx_TIMERx = 0x1, // QTIMERx_TIMERx
  ALT2_FLEXPWMx_PWMBx = 0x2, // FLEXPWMx_PWMBx
  ALT3_ARM_TRACEx = 0x3, // ARM_TRACEx
  ALT4_FLEXIOx_FLEXIOx = 0x4, // FLEXIOx_FLEXIOx
  ALT5_GPIOx_IOx = 0x5, // GPIOx_IOx
  ALT6_SRC_BOOT_CFGx = 0x6, // SRC_BOOT_CFGx
  ALT8_ENET = 0x8 // ENET2_TX_ER
} muxmode_e;

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
} mux_fieldoffs_e;

typedef enum
{
  SRE_0_SLOW_SLEW_RATE = 0x0,
  SRE_1_FAST_SLEW_RATE = 0x1
} muxpad_sre_e; // Slew Rate Field

typedef enum
{
  PAD_DSE_DISABLED = 0x0,
  PAD_DSE_R0_150O_3_3_V__260O_1_8V = 0x1,
  PAD_DSE_R0_2 = 0x2,
  PAD_DSE_R0_3 = 0x3,
  PAD_DSE_R0_4 = 0x4,
  PAD_DSE_R0_5 = 0x5,
  PAD_DSE_R0_6 = 0x6,
  PAD_DSE_R0_7 = 0x7,
} muxpad_dse_e; // Drive Strength Field

typedef enum
{
  PAD_SPEED_50MHz = 0x0,
  PAD_SPEED_100MHz = 0x1,
  PAD_SPEED_150MHz = 0x2,
  PAD_SPEED_200MHz = 0x3
} muxpad_speed_e; // Speed Field

typedef enum
{
  ODE_0_DRAIN_DISABLED = 0x0,
  ODE_1_DRAIN_ENABLED = 0x1
} muxpad_ode_e; // Open Drain Field

typedef enum
{
  PKE_0_KEEPER_DISABLED = 0x0,
  PKE_1_KEEPER_ENABLED = 0x1
} muxpad_pke_e; // Pull/Keep Enable Field

typedef enum
{
  PAD_KEEPER = 0x0,
  PAD_PULLER = 0x1
} muxpad_pue_e; // Pull/Keep Select Field

typedef enum
{
  PAD_CONF_100KOHM_DOWN = 0x0,
  PAD_CONF_47KOHM_UP = 0x1,
  PAD_CONF_100KOHM_UP = 0x2,
  PAD_CONF_22KOHM_UP = 0x3,
} muxpad_pus_e; // Pull Up/Down Config. Field

typedef enum
{
  HYS_0_DISABLED = 0x0,
  HYS_1_ENABLED = 0x1
} muxpad_hys_e; // Hysteresis Enable Field

typedef enum
{
  RO,
  WO,
  RW,
  W1C
} access_e;

typedef enum
{
  LED
} devicetype_e;

typedef enum
{
  GPIO_AD_B0,
  GPIO_AD_B1,
  GPIO_B0,
  GPIO_B1,
  GPIO_SD_B0,
  GPIO_SD_B1
} muxpad_cr_e;

typedef union {
  muxpad_sre_e   slew_rate;
  muxpad_dse_e   drive_strength;
  muxpad_speed_e speed;
  muxpad_ode_e   open_drain;
  muxpad_pke_e   pull_keep_enable;
  muxpad_pue_e   pull_keep_select;
  muxpad_pus_e   pull_up_down_conf;
  muxpad_hys_e   hysteresis_enable;
  muxmode_e      selected_mux_mode;
} muxpad_fields_u;

typedef struct {
  vuint32_t *     mux_pad_addr;
  vuint32_t *     pad_pad_addr;
  muxpad_fields_u current_pad_type;
} muxpad_context_s;

typedef struct {
  muxpad_context_s mux_pad_context;
  muxmode_e        mux_mode;
  uint8_t          ctrl_pos;
} muxdev_s;

// 32bit + 32bit + 24bit
// 11 bytes
#define MUXDEVICE_BYTESIZE ((uint8_t)0xc) // an extra byte for good measure

void
init_device_muxmode(muxdev_s *   mux_device,
                    vuint32_t *  sw_mux,
                    vuint32_t *  sw_pad,
                    muxpad_dse_e dse_opt,
                    uint8_t      ctrl_pos,
                    muxmode_e    mux_mode);

void
set_muxmode(muxdev_s * mux_device, muxmode_e mux_mode);

#endif // IOMUX_CONTROLLER_H