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
} EMuxMode;

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
  GPIO_AD_B0,
  GPIO_AD_B1,
  GPIO_B0,
  GPIO_B1,
  GPIO_SD_B0,
  GPIO_SD_B1
} EPadCR;

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
  SPadContext mux_pad_context;
  EMuxMode    mux_mode;
  uint8_t     ctrl_pos;
} SStoredMUXDevice;

// 32bit + 32bit + 24bit
// 11 bytes
#define MUXDEVICE_BYTESIZE ((uint8_t)0xc) // an extra byte for good measure

void
init_device_muxmode(SStoredMUXDevice * mux_device,
                    vuint32_t *        sw_mux,
                    vuint32_t *        sw_pad,
                    EBitMuxPad_DSE     dse_opt,
                    uint8_t            ctrl_pos,
                    EMuxMode           mux_mode);

void
set_muxmode(SStoredMUXDevice * mux_device, EMuxMode mux_mode);

#endif // IOMUX_CONTROLLER_H