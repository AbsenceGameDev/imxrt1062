/**
 * @file      clk_control.h
 * @author    Ario@Permadev
 * @brief
 * @version   0.1
 * @date      2021-08-29
 *
 * @copyright Copyright (c) 2021, MIT-License included in project toplevel dir
 *
 */

#ifndef CLK_CONTROL_H
#define CLK_CONTROL_H

#include "system_memory_map.h"

typedef enum
{
  IPG_ROOT = 0x0,
  OSC_ROOT = 0x1
} perclk_src_e;
#define CCM_SCMUX1_SET(x) CCM_C_SCMR1 = (x)
#define CCM_SCMUX1_DIV(x) (x & 0b111111)
#define CCM_SCMUX1_DIV_SET(x)                                                  \
  CCM_C_SCMR1 = ((CCM_C_SCMR1 & ~0b111111) | CCM_SCMUX1_DIV(x))
#define CCM_PERCLK_SRC(x) ((x & 0x1) << 0x6)
#define CCM_SLCT_PERCLK_SRC(x)                                                 \
  CCM_C_SCMR1 = ((CCM_C_SCMR1 & ~(0x1 << 0x6)) | CCM_PERCLK_SRC(x))

#define CCM_C_CGR1_GPT1(n) ((uint32_t)(((n)&0x03) << 20))

#define CCGR_ON              ((uint16_t)0x1)
#define CCGR_OFF             ((uint16_t)0x0)
#define CCM_CCGR6_QTIMER1(x) (~(0x1 << 0x1a) & ((x) << 0x1a))

#define TMR_CTRL_CM(n)            ((uint16_t)(((n)&0x07) << 13))
#define TMR_CTRL_PCS(n)           ((uint16_t)(((n)&0x0F) << 9))
#define TMR_CTRL_SCS(n)           ((uint16_t)(((n)&0x03) << 7))
#define TMR_CTRL_ONCE             ((uint16_t)(1 << 6))
#define TMR_CTRL_LENGTH           ((uint16_t)(1 << 5))
#define TMR_CTRL_DIR_DOWN         ((uint16_t)(1 << 4))
#define TMR_CTRL_COINIT           ((uint16_t)(1 << 3))
#define TMR_CTRL_OUTMODE(n)       ((uint16_t)(((n)&0x07) << 0))
#define TMR_SCTRL_TCF             ((uint16_t)(1 << 15))
#define TMR_SCTRL_TCFIE           ((uint16_t)(1 << 14))
#define TMR_SCTRL_TOF             ((uint16_t)(1 << 13))
#define TMR_SCTRL_TOFIE           ((uint16_t)(1 << 12))
#define TMR_SCTRL_IEF             ((uint16_t)(1 << 11))
#define TMR_SCTRL_IEFIE           ((uint16_t)(1 << 10))
#define TMR_SCTRL_IPS             ((uint16_t)(1 << 9))
#define TMR_SCTRL_INPUT           ((uint16_t)(1 << 8))
#define TMR_SCTRL_CAPTURE_MODE(n) ((uint16_t)(((n)&0x03) << 6))
#define TMR_SCTRL_MSTR            ((uint16_t)(1 << 5))
#define TMR_SCTRL_EEOF            ((uint16_t)(1 << 4))
#define TMR_SCTRL_VAL             ((uint16_t)(1 << 3))
#define TMR_SCTRL_FORCE           ((uint16_t)(1 << 2))
#define TMR_SCTRL_OPS             ((uint16_t)(1 << 1))
#define TMR_SCTRL_OEN             ((uint16_t)(1 << 0))
#define TMR_CSCTRL_DBG_EN(n)      ((uint16_t)(((n)&0x03) << 14))
#define TMR_CSCTRL_FAULT          ((uint16_t)(1 << 13))
#define TMR_CSCTRL_ALT_LOAD       ((uint16_t)(1 << 12))
#define TMR_CSCTRL_ROC            ((uint16_t)(1 << 11))
#define TMR_CSCTRL_TCI            ((uint16_t)(1 << 10))
#define TMR_CSCTRL_UP             ((uint16_t)(1 << 9))
#define TMR_CSCTRL_TCF2EN         ((uint16_t)(1 << 7))
#define TMR_CSCTRL_TCF1EN         ((uint16_t)(1 << 6))
#define TMR_CSCTRL_TCF2           ((uint16_t)(1 << 5))
#define TMR_CSCTRL_TCF1           ((uint16_t)(1 << 4))
#define TMR_CSCTRL_CL2(n)         ((uint16_t)(((n)&0x03) << 2))
#define TMR_CSCTRL_CL1(n)         ((uint16_t)(((n)&0x03) << 0))
#define TMR_FILT_FILT_CNT(n)      ((uint16_t)(((n)&0x07) << 8))
#define TMR_FILT_FILT_PER(n)      ((uint16_t)(((n)&0xFF) << 0))
#define TMR_DMA_CMPLD2DE          ((uint16_t)(1 << 2))
#define TMR_DMA_CMPLD1DE          ((uint16_t)(1 << 1))
#define TMR_DMA_IEFDE             ((uint16_t)(1 << 0))

#endif // CLK_CONTROL_H