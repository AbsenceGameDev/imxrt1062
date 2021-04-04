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
 *  ... Read the 52.6.1 sub-chapter in the ref manual
 *
 *  NOTE: An IP bus write access to the GPT Control Register (GPT_CR) and the
 *        GPT Output Compare Register1 (GPT_OCR1) results in one cycle of wait
 *        state, while other valid IP-bus accesses incur 0 wait states.
 *
 **/

typedef enum
{
  FREERUNMODE_E = 0b00,
  RESTARTMODE_E = 0b01
} gpt_run_mode_e;

typedef enum
{
  PIT_SPEED_50MHz = 0x0,
  PIT_SPEED_100MHz = 0x1,
  PIT_SPEED_150MHz = 0x2,
  PIT_SPEED_200MHz = 0x3
} EPITSpeed; // Speed Field

// General
#define GPT_IR_SET_EN(x) (~0x1f) | ((x)&0x1f)
#define GPT_CR_SET_EN(x) (~0x1) | ((x)&0x1)
#define GPT_CR_SET_OM1(x) ~(0x7 << 0x14) | (((x)&0x7) << 0x14)
#define GPT_CR_SET_OM2(x) ~(0x7 << 0x17) | (((x)&0x7) << 0x17)
#define GPT_CR_SET_OM3(x) ~(0x7 << 0x1a) | (((x)&0x7) << 0x1a)
#define GPT_CR_IM_CLR ~(0xf << 0x10) // clear [19,16]
#define GPT_CR_IM1_SET(x) ~(0x3 << 0x10) | (((x)&0x3) << 0x10) // set [17,16]
#define GPT_CR_IM2_SET(x) ~(0x3 << 0x12) | (((x)&0x3) << 0x12) // set [19,18]
#define GPT_CR_CLKSRC(SRC) ~(0x7 << 0x6) | ((SRC & 0x7) << 0x6)
#define GPT_CR_SWR(x) ~(0x1 << 0xf) | (((x)&0x1) << 0xf)
#define GPT_CR_SET_ENMOD(x) ~(0x1 << 0x1) | (((x)&0x1) << 0x1)
#define GPT_SR_CLR ~0x1f
#define GPT_CR_MODE(gpt_run_mode) ~(0x1 << 0x9) | ((gpt_run_mode & 0x1) << 0x9)

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
#define GPT2_CR_CLKSRC(SRC) GPT2_CR &= ~(0x7 << 0x9) | ((SRC & 0x7) << 0x6)
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

// // Some defines to set enable GPTx at the Clock Controlelr Module (CCM)
/** @brief Set gpt2 bus clock field in CCM Clock gating register 0, p.1085 */
#define GPT2_SET_BUS(x) (uint32_t)(((x)&0b11) << 24)
/** @brief set gpt2 bus clock field in CCM Clock gating register 0, p.1085 */
#define GPT2_SET_SERIAL(x) (uint32_t)(((x)&0b11) << 26)
#define CCM_C_GPT2_EN                                                          \
  CCM_C_CGR0 |= GPT2_SET_BUS(CLK_ON__NO_STOP) | GPT2_SET_SERIAL(CLK_ON__NO_STOP)

/** @brief Set gpt1 bus clock field in CCM Clock gating register 1, p.1085 */
#define GPT1_SET_BUS(x) (uint32_t)(((x)&0b11) << 20)
/** @brief set gpt1 bus clock field in CCM Clock gating register 1, p.1085 */
#define GPT1_SET_SERIAL(x) (uint32_t)(((x)&0b11) << 22)
#define CCM_C_GPT1_EN                                                          \
  CCM_C_CGR1 |= GPT1_SET_BUS(CLK_ON__NO_STOP) | GPT1_SET_SERIAL(CLK_ON__NO_STOP)

typedef enum
{
  GPT1_E,
  GPT2_E,
  PIT0_E,
  PIT1_E,
  PIT2_E,
  PIT3_E,
} timer_e;

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
} timeunit_u;

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
} timetype_e;

typedef struct {
  timeunit_u count;
  uint16_t   steps; // steps in this case being wrap
} timer_s;

typedef void (*timer_manager_cb)(void);

typedef struct {
  timer_e          gpt_x;
  gpt_ocr_e        ocr_ch;
  timetype_e       time_type;
  timer_s          time_container;
  clk_src_e        gpt_clk;
  uint32_t         compval;
  EPITSpeed        speedfield;
  timer_manager_cb callback;
} timer_manager_t;

extern timer_manager_t
    glob_gptman[6]; /** idx [0,2] == gpt1_OCRidx, idx [3,5] == gpt2_OCRidx, */

extern vuint32_t *
    glob_gpt_ptrs[6]; /** idx [0,2] == gpt1_OCRidx, idx [3,5] == gpt2_OCRidx */

void
init_gptman_arr();

void
init_gptman(timer_manager_t * gptman,
            timer_e           gpt_x,
            gpt_ocr_e         ocr_ch,
            clk_src_e         gpt_clk,
            timetype_e        time_type,
            timer_s           time_container,
            uint32_t          compval,
            timer_manager_cb  callback);

void
__slct_clksrc_gpt__(timer_manager_t * timer);

void
__set_callback_gpt__(timer_manager_t * timer, timer_manager_cb callback);

// Time clamp function delcarations,
// used to clamp values to fit in a 32bit register.
// magic numbers calculated by:
// max_cycles_for_unit = max_register_value / cycle_rate_per_unit
uint32_t
__time_clamp_24MHz__(uint32_t intime, timetype_e ttype);

uint32_t
__time_clamp_50MHz__(uint32_t intime, timetype_e ttype);

uint32_t
__time_clamp_100MHz__(uint32_t intime, timetype_e ttype);

uint32_t
__time_clamp_150MHz__(uint32_t intime, timetype_e ttype);

uint32_t
__time_clamp_200MHz__(uint32_t intime, timetype_e ttype);

// Time resolve function delcarations,
// used to resolves input values to cycle_count for a 32bit cycle register.
// magic numbers calculated by:
// max_cycles_for_unit = max_register_value / cycle_rate_per_unit
uint32_t
__resolve_time_24MHz__(timer_manager_t * gptimer_mgr);

uint32_t
__resolve_time_50MHz__(timer_manager_t * timer_mgr);

uint32_t
__resolve_time_100MHz__(timer_manager_t * timer_mgr);

uint32_t
__resolve_time_150MHz__(timer_manager_t * timer_mgr);

uint32_t
__resolve_time_200MHz__(timer_manager_t * timer_mgr);

void
__set_comparator_gpt__(timer_manager_t * timer);

void
set_time(timer_manager_t * timer, timetype_e time_type, gpt_ocr_t compareval);

void
callback_gpt1(void);

void
callback_gpt2(void);

void
__setup_gpt2__();

#endif // GP_TIMER_H

#ifndef PI_TIMER_H
#define PI_TIMER_H

/**
 * PIT Frequency-settings:
 * LOW  - 50MHz   : 20    nanosecond (granularity)
 * MID  - 100MHz  : 10    nanosecond (granularity)
 * HIGH - 150MHz  : 6.667 nanosecond (granularity)
 * MAX  - 200MHz  : 5     nanosecond (granularity)
 **/

// PIT Module Control Register (MCR)
typedef enum
{
  PIT_RUN_IN_DEBUG = 0x0,
  PIT_STOP_IN_DEBUG = 0x1
} EFreezeDebugPIT;

// PIT Module Control Register (MCR)
typedef enum
{
  MCR_RESET = 0x0
} EMCR;

#define PIT_EN(x) ~(0x1 << 0x1) | ((x & 0x1) << 0x1)
#define PIT_FREEZE(x) ~0x1 | (x & 0x1)
#define PIT_MCR_SET(x) PIT_MCR &= (x)

// PIT Upper Lifetime Timer Register (LTMR64H)
/** Shows value of timer 1. If timer 1 is read at t1 it shows value of timer 0 */
#define PIT_HIGH_TIMER_READ PIT_LTMR64H

// PIT Lower Lifetime Timer Register (LTMR64L)
/** Shows value of timer 0 at the time LTMR64H was read. Thus will only update
 * if LTMR64H is read */
#define PIT_LOW_TIMER_READ PIT_LTMR64L

// Timer Load Value Register (LDVAL0 - LDVAL3), ch. 53.9.5, p 2982
// Register | Offset
// LDVAL0   | 100h
// LDVAL1   | 110h
// LDVAL2   | 120h
// LDVAL3   | 130h
// These registers select the timeout period for the timer interrupts.
#define PIT_LOADVAL0_SET(x) PIT_LDVAL0 = (x)&0xffffffff
#define PIT_LOADVAL1_SET(x) PIT_LDVAL1 = (x)&0xffffffff
#define PIT_LOADVAL2_SET(x) PIT_LDVAL2 = (x)&0xffffffff
#define PIT_LOADVAL3_SET(x) PIT_LDVAL3 = (x)&0xffffffff

// Current Timer Value Register (CVAL0 - CVAL3), ch. 53.9.5, p 2982
// Register | Offset
// CVAL0   | 104h
// CVAL1   | 114h
// CVAL2   | 124h
// CVAL3   | 134h
// These registers indicate the current timer position.
// NOTE:
// •  If the timer is disabled, do not use this field because
//    its value is unreliable.
// •  The timer uses a downcounter. The timer values are frozen
//    in the Debug mode ifMCR[FRZ] is set
#define PIT_CURVAL0_READ PIT_CVAL0
#define PIT_CURVAL1_READ PIT_CVAL1
#define PIT_CURVAL2_READ PIT_CVAL2
#define PIT_CURVAL3_READ PIT_CVAL3

// Timer Control Register (TCTRL0 - TCTRL3). Ch.53.9.7  p. 2984
// bits [31, 3] == Reserved
// bits [2] == Chain Mode
// bits [1] == Timer Interrupt Enable
// bits [0] == Timer Enable

typedef enum
{
  PIT_NOT_CHAINED = 0x0,
  PIT_CHAINED = 0x1
} EPITChainMode;

typedef enum
{
  PIT_TIMER_N_IRQ_DISABLED = 0x0,
  PIT_IRQ_ON_TIF = 0x1
} EPITIrqEnable;

typedef enum
{
  PIT_TIMER_00 = 0x0,
  PIT_TIMER_01 = 0x1,
  PIT_TIMER_02 = 0x2,
  PIT_TIMER_03 = 0x3,
} EPITTimer;

/**
 * @brief Chain Mode
 * @details When activated, timer n-1 needs to expire before timer n (n is > 0)
 * can decrement by 1.
 * Timer 0 cannot be chained.
 * 0b - Timer is not chained.
 * 1b - Timer is chained to a previous timer.
 * For example, for channel 2, if this field is set:
 * Timer 2 is chained to Timer 1
 **/
#define PIT_TCTRL_CHAIN(x) ~(0x1 << 0x2) | ((x & 0x1) << 0x2)

/**
 * @brief Timer Interrupt Enable
 * @details When an interrupt is pending, or if TFLGn[TIF] is set, enabling the
 * interrupt causes an interrupt event. \n
 * To avoid this, the associated TFLGn[TIF] must be cleared first.
 * 0b - Interrupt requests from Timer n are disabled.
 * 1b - Interrupt is requested whenever TIF is set.
 **/
#define PIT_TCTRL_IRQ(x) ~(0x1 << 0x1) | ((x & 0x1) << 0x1)

/**
 * @brief Timer Enable
 * @details Enables or disables the timer.
 * 0b - Timer n is disabled.
 * 1b - Timer n is enabled
 **/
#define PIT_TCTRL_TIMER(x) ~0x1 | (x & 0x1)

#define PIT_TCTRL0_CHAIN_SET(x) PIT_TCTRL0 &= PIT_TCTRL_CHAIN(x)
#define PIT_TCTRL0_IRQ_EN(x) PIT_TCTRL0 &= PIT_TCTRL_IRQ(x)
#define PIT_TCTRL0_TIMER_EN(x) PIT_TCTRL0 &= PIT_TCTRL_TIMER(x)

#define PIT_TCTRL1_CHAIN_SET(x) PIT_TCTRL1 &= PIT_TCTRL_CHAIN(x)
#define PIT_TCTRL1_IRQ_EN(x) PIT_TCTRL1 &= PIT_TCTRL_IRQ(x)
#define PIT_TCTRL1_TIMER_EN(x) PIT_TCTRL1 &= PIT_TCTRL_TIMER(x)

#define PIT_TCTRL2_CHAIN_SET(x) PIT_TCTRL2 &= PIT_TCTRL_CHAIN(x)
#define PIT_TCTRL2_IRQ_EN(x) PIT_TCTRL2 &= PIT_TCTRL_IRQ(x)
#define PIT_TCTRL2_TIMER_EN(x) PIT_TCTRL2 &= PIT_TCTRL_TIMER(x)

#define PIT_TCTRL3_CHAIN_SET(x) PIT_TCTRL3 &= PIT_TCTRL_CHAIN(x)
#define PIT_TCTRL3_IRQ_EN(x) PIT_TCTRL3 &= PIT_TCTRL_IRQ(x)
#define PIT_TCTRL3_TIMER_EN(x) PIT_TCTRL3 &= PIT_TCTRL_TIMER(x)

// Timer Flag Register (TFLG0 - TFLG3). Ch.53.9.8  p. 2985
// bits [31, 1] == Reserved
// bits [0] == Timer Interrupt Flag
// PIT_TFLG0
// PIT_TFLG1
// PIT_TFLG2
// PIT_TFLG3
//

/**
 * @brief Timer Interrupt Flag
 * @details Sets to 1 at the end of the timer period. \n
 * Writing 1 to this flag clears it and writing 0 has no effect. \n
 * If enabled, or, when TCTRLn[TIE] = 1, TIF causes an interrupt request. \n
 * 0b - Timeout has not yet occurred. \n
 * 1b - Timeout has occurred. \n
 **/
#define PIT_TFLG(x) ~0x1 | (x & 0x1)
#define PIT_TFLG0_CLR PIT_TFLG0 &= PIT_TFLG(0x1)
#define PIT_TFLG1_CLR PIT_TFLG1 &= PIT_TFLG(0x1)
#define PIT_TFLG2_CLR PIT_TFLG2 &= PIT_TFLG(0x1)
#define PIT_TFLG3_CLR PIT_TFLG3 &= PIT_TFLG(0x1)

/**
 * @brief Setup a Pit Timer
 * @details Sets up a pit time based on the inputted pit_mgr
 * @param pit_mgr Type: timer_manager_t
 * @note Will need to set the appropriate members of pit_mgr
 *       before calling this function
 **/
void
setup_pit_timer(timer_manager_t * pit_mgr, EPITTimer pit_timerx);

#endif // PI_TIMER_H