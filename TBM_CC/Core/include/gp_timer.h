#ifndef GP_TIMER_H
#define GP_TIMER_H
// General Purpose Timers

/**
 * 52.6.1  Selecting the Clock Source
 * The CLKSRC field in the GPT_CR register selects the clock source.
 * The CLKSRC field value should be changed only after disabling the GPT
 * (EN=0).
 * The software sequence to be followed while changing clock source is:
 *  1.  Disable GPT by setting EN=0 in GPT_CR register.
 *  2.  Disable GPT interrupt register (GPT_IR) (set fields [5,0] == 0)
 *  3.  Configure Output Mode to unconnected/ disconnected—Write zeros in OM3,
 *      OM2,and OM1 in GPT_CR
 *  4.  Disable Input Capture Modes—Write zeros in IM1 and IM2 in GPT_CR
 *  5.  Change clock source CLKSRC to the desired value in GPT_CR register.
 *  6.  Assert the SWR bit in GPT_CR register.
 *  7.  Clear GPT status register (GPT_SR) (i.e., w1c). (set fields [5,0] == 0)
 *  8.  Set ENMOD=1 in GPT_CR register, to bring GPT counter to 0x00000000.
 *  9.  Enable GPT (EN=1) in GPT_CR register.
 *  10. Enable GPT interrupt register (GPT_IR) (set fields [5,0] == 1)
 *
 *  NOTE: An IP bus write access to the GPT Control Register (GPT_CR) and the
 *        GPT Output Compare Register1 (GPT_OCR1) results in one cycle of wait
 *        state, while other valid IP-bus accesses incur 0 wait states.
 *
 * Absolute hex addresses relevant to GPT and above steps
 * GPT1_CR = 0x401ec000
 * GPT1_SR = 0x401ec008
 * GPT1_IR = 0x401ec00c
 * GPT2_CR = 0x401f0000
 * GPT2_SR = 0x401f0008
 * GPT2_IR = 0x401f000c
 **/

#include "system_memory_map.h"
// GPT1 macros
#define GPT1_IR_EN(x) (GPT1_IR &= (~0x1f) | ((x)&0x1f))
#define GPT1_CR_EN(x) (GPT1_CR &= (~0x1) | ((x)&0x1))
#define GPT1_CR_SET_OM1(x) (GPT1_CR &= (~(0x7 << 0x14) | (((x)&0x7) << 0x14)))
#define GPT1_CR_SET_OM2(x) (GPT1_CR &= (~(0x7 << 0x17) | (((x)&0x7) << 0x17)))
#define GPT1_CR_SET_OM3(x) (GPT1_CR &= (~(0x7 << 0x1a) | (((x)&0x7) << 0x1a)))
#define GPT1_CR_IM_CLR GPT1_CR &= ~(0xf << 0x10) // clear [19,16]
#define GPT1_CR_CLKSRC(SRC) GPT1_CR &= ~(0x7 << 0x6) | ((SRC & 0x7) << 0x6)
#define GPT1_CR_SWR(x) (GPT1_CR &= ~(0x1 << 0xf) | (((x)&0x1) << 0xf))
#define GPT1_CR_SET_ENMOD(x) (GPT1_CR &= ~(0x1 << 0x1) | (((x)&0x1) << 0x1))
#define GPT1_SR_CLR GPT1_SR &= ~0x1f

// GPT2 macros
#define GPT2_IR_EN(x) (GPT2_IR &= (~0x1f) | ((x)&0x1f))
#define GPT2_CR_EN(x) (GPT2_CR &= (~0x1) | ((x)&0x1))
#define GPT2_CR_SET_OM1(x) (GPT2_CR &= (~(0x7 << 0x14) | (((x)&0x7) << 0x14)))
#define GPT2_CR_SET_OM2(x) (GPT2_CR &= (~(0x7 << 0x17) | (((x)&0x7) << 0x17)))
#define GPT2_CR_SET_OM3(x) (GPT2_CR &= (~(0x7 << 0x1a) | (((x)&0x7) << 0x1a)))
#define GPT2_CR_IM_CLR GPT2_CR &= ~(0xf << 0x10) // clear [19,16]
#define GPT2_CR_CLKSRC(SRC) GPT2_CR &= ~(0x7 << 0x6) | ((SRC & 0x7) << 0x6)
#define GPT2_CR_SWR(x) (GPT2_CR &= ~(0x1 << 0xf) | (((x)&0x1) << 0xf))
#define GPT2_CR_SET_ENMOD(x) (GPT2_CR &= ~(0x1 << 0x1) | (((x)&0x1) << 0x1))
#define GPT2_SR_CLR GPT2_SR &= ~0x1f

typedef enum
{
  GPT1_E,
  GPT2_E
} EGptimer;

typedef enum
{
  GPT_NO_CLK = 0b000,
  GPT_IPG_CLK = 0b001,
  GPT_IPG_CLK_HIFREQ = 0b010,
  GPT_EXT_CLK = 0b011,
  GPT_IPG_CLK_32K = 0b100,
  GPT_IPG_CLK_24M = 0b101 // XTALOSC
} EClockSrc;

typedef struct {
  EGptimer  gpt_x;
  EClockSrc gpt_clk;
} gp_timer;

const void
slct_clksrc_gpt(gp_timer * timer);

#endif // GP_TIMER_H