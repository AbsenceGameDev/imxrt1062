// General Purpose Timers
#ifndef GP_TIMER_H
#define GP_TIMER_H

#include "irq_handler.h"
#include "system_memory_map.h"

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
 *
 **/

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

/**
 * @brief Clock Gating Register enum
 *
 * CGR value | Clock Activity Description
 * @param CLK_OFF__ALL_MODES Clock is off during all modes.
 *                           Stop enter hardware handshake is disabled.
 * @param CLK_ON__RUN Clock is on in run mode, but off in WAIT and STOP modes
 * @param CLK_ON__NO_STOP Clock is on during all modes, except STOP mode.
 **/
typedef enum
{
  CLK_OFF__ALL_MODES = 0b00,
  CLK_ON__RUN = 0b01,
  CLK_ON__NO_STOP = 0b11
} clk_gate_reg_e;

// Some defines to set enable GPTx at the Clock Controlelr Module (CCM)
#define CCM_C_CGR0_GPT2_SERIAL(n) ((uint32_t)(((n)&0b11) << 26))
#define CCM_C_CGR0_GPT2_BUS(n) ((uint32_t)(((n)&0b11) << 24))
#define CCM_C_GPT2_EN                                                          \
  CCM_C_CGR0 |= CCM_C_CGR0_GPT2_BUS(CLK_ON__NO_STOP) |                         \
                CCM_C_CGR0_GPT2_SERIAL(CLK_ON__NO_STOP)

#define CCM_C_CGR1_GPT1_SERIAL(n) ((uint32_t)(((n)&0b11) << 22))
#define CCM_C_CGR1_GPT1_BUS(n) ((uint32_t)(((n)&0b11) << 20))
#define CCM_C_GPT1_EN                                                          \
  CCM_C_CGR1 |= CCM_C_CGR1_GPT1_BUS(CLK_ON__NO_STOP) |                         \
                CCM_C_CGR1_GPT1_SERIAL(CLK_ON__NO_STOP)

typedef enum
{
  GPT1_E,
  GPT2_E
} gptimer_e;

typedef enum
{
  OCR_CH1,
  OCR_CH2,
  OCR_CH3
} gpt_ocr_e;
typedef uint32_t gpt_ocr_t;

typedef enum
{
  GPT_NO_CLK = 0b000,
  GPT_IPG_CLK = 0b001,
  GPT_IPG_CLK_HIFREQ = 0b010,
  GPT_EXT_CLK = 0b011,
  GPT_IPG_CLK_32K = 0b100,
  GPT_IPG_CLK_24M = 0b101 // XTALOSC
} clk_src_e;

typedef uint32_t days_t; // ((60²)*24) (s)
typedef uint32_t hours_t; // 60²(s)
typedef uint32_t minutes_t; // 60(s)
typedef uint32_t seconds_t; // 1(s)
typedef uint32_t millis_t; // 10⁻³(s)
typedef uint32_t micros_t; // 10⁻⁶(s)
typedef uint32_t zeptos_t; // 10⁻⁷(s)
typedef uint32_t yoctos_t; // 10⁻⁸(s)
typedef uint32_t nanos_t; // 10⁻⁹(s)

typedef union {
  days_t    d;
  hours_t   h;
  minutes_t m;
  seconds_t s;
  millis_t  ms; // milli-seconds
  micros_t  us; // micro-seconds
  zeptos_t  zs; // zepto-seconds
  yoctos_t  ys; // yocto-seconds
  nanos_t   ns; // nano-seconds
} gp_timer_u;

typedef enum
{
  DAYS_E,
  HOURS_E,
  MINUTES_E,
  SECONDS_E,
  MILLIS_E, // milli-seconds
  MICROS_E, // micro-seconds
  ZEPTOS_E, // zepto-seconds
  YOCTOS_E, // yocto-seconds
  NANOS_E // nano-seconds
} gp_timetype_e;

typedef struct {
  gp_timer_u count;
  uint16_t   steps; // steps in this case being wrap
} gp_timer_s;

typedef void * (*timer_manager_cb)(void);

typedef struct {
  gptimer_e        gpt_x;
  uint32_t         compval;
  timer_manager_cb callback;
  clk_src_e        gpt_clk;
  gp_timer_s       time_container;
  gp_timetype_e    time_type;
  gpt_ocr_e        ocr_ch;
} gpt_manager;

extern gpt_manager
    glob_gptman[6]; /** idx [0,2] == gpt1_OCRidx, idx [3,5] == gpt2_OCRidx, */

extern vuint32_t *
    glob_gpt_ptrs[6]; /** idx [0,2] == gpt1_OCRidx, idx [3,5] == gpt2_OCRidx */

void
init_gptman();

void
__slct_clksrc_gpt__(gpt_manager * timer);

void
__set_callback_gpt__(gpt_manager * timer, timer_manager_cb callback);

void
__set_comparator_gpt__(gpt_manager * timer,
                       gpt_ocr_t     compareval,
                       gpt_ocr_e     channel);

void
set_time(gpt_manager * timer, gp_timetype_e time_type, gpt_ocr_t compareval);

void
callback_gpt1(void);

void
callback_gpt2(void);

#endif // GP_TIMER_H