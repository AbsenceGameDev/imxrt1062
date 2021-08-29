/**
 * @file      timer_manager.h
 * @author    Ario@Permadev
 * @brief
 * @version   0.1
 * @date      2021-08-29
 *
 * @copyright Copyright (c) 2021, MIT-License included in project toplevel dir
 *
 */

#ifndef BASE_TIMER_H
#define BASE_TIMER_H

#include "clk_control.h"
#include "irq_handler.h"
#include "system_heap.h"
#include "system_memory_map.h"

typedef enum
{
  SRC_NO_CLK = 0b000,
  SRC_IPG_CLK = 0b001,
  SRC_IPG_CLK_HIFREQ = 0b010,
  SRC_EXT_CLK = 0b011,
  SRC_IPG_CLK_32K = 0b100,
  SRC_IPG_CLK_24M = 0b101 // XTALOSC
} clk_src_e;

typedef uint32_t days_t;    // ((60²)*24) (s)
typedef uint32_t hours_t;   // 60²(s)
typedef uint32_t minutes_t; // 60(s)
typedef uint32_t seconds_t; // 1(s)
typedef uint32_t millis_t;  // 10⁻³(s)
typedef uint32_t micros_t;  // 10⁻⁶(s)
typedef uint32_t zettos_t;  // 10⁻⁷(s)
typedef uint32_t yottos_t;  // 10⁻⁸(s)
typedef uint32_t nanos_t;   // 10⁻⁹(s)

typedef union {
  days_t    d;
  hours_t   h;
  minutes_t m;
  seconds_t s;
  millis_t  ms; // milli-seconds
  micros_t  us; // micro-seconds
  zettos_t  zs; // zetto-seconds
  yottos_t  ys; // yotto-seconds
  nanos_t   ns; // nano-seconds
} timeunit_u;

typedef enum
{
  DAYS_E,
  HOURS_E,
  MINUTES_E,
  SECONDS_E,
  MILLIS_E, // milli-seconds, 10E-3
  MICROS_E, // micro-seconds, 10E-6
  ZETTOS_E, // zetto-seconds, 10E-7
  YOTTOS_E, // yotto-seconds, 10E-8
  NANOS_E   // nano-seconds,  10E-9
} timetype_e;

typedef struct {
  timeunit_u count;
  uint16_t   steps; // steps in this case being wrap
} timer_s;

typedef void (*timer_manager_cb)(void);

typedef enum
{
  GPT_E,
  PIT_E,
  QUAD_E,
  WDOG_E,
  RTWDOG_E
} timer_type_e;

typedef struct {
  timer_type_e type;
  void *       context;
} timer_context_t;

typedef struct {
  timetype_e        time_type;
  uint32_t          targetval;
  timer_s           time_container;
  clk_src_e         clk_src;
  timer_context_t * timer_ctx;
  timer_manager_cb  callback;
} timer_manager_t;

// Time clamp function delcarations,
// used to clamp values to fit in a 32bit register.

/**
 * @brief   Clamps values to fit 24MHz timer counter
 * @details Clamps values if they exceed max limit of the count cycle
 *          registers 24mhz cycles. Clamps them using arithmetic logic,
 *          instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value,
 *
 * @note magic numbers calculated by:
 * @note max_cycles_for_unit = max_register_value / cycle_rate_per_unit
 **/
uint32_t
__time_clamp_24MHz__(uint32_t intime, timetype_e ttype);

/**
 * @brief   Clamps values to fit 50MHz timer counter
 * @details Clamps values if they exceed max limit of the count cycle
 *          registers 50MHz cycles. Clamps them using arithmetic logic,
 *          instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value,
 *
 * @note magic numbers calculated by:
 * @note max_cycles_for_unit = max_register_value / cycle_rate_per_unit
 **/
uint32_t
__time_clamp_50MHz__(uint32_t intime, timetype_e ttype);

/**
 * @brief   Clamps values to fit 100MHz timer counter
 * @details Clamps values if they exceed max limit of the count cycle
 *          registers 100MHz cycles. Clamps them using arithmetic logic,
 *          instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value,
 *
 * @note magic numbers calculated by:
 * @note max_cycles_for_unit = max_register_value / cycle_rate_per_unit
 **/
uint32_t
__time_clamp_100MHz__(uint32_t intime, timetype_e ttype);

/**
 * @brief   Clamps values to fit 150MHz timer counter
 * @details Clamps values if they exceed max limit of the count cycle
 *          registers 150MHz cycles. Clamps them using arithmetic logic,
 *          instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value,
 *
 * @note magic numbers calculated by:
 * @note max_cycles_for_unit = max_register_value / cycle_rate_per_unit
 **/
uint32_t
__time_clamp_150MHz__(uint32_t intime, timetype_e ttype);

/**
 * @brief   Clamps values to fit 200MHz timer counter
 * @details Clamps values if they exceed max limit of the count cycle
 *          registers 200MHz cycles. Clamps them using arithmetic logic,
 *          instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value,
 *
 * @note magic numbers calculated by:
 * @note max_cycles_for_unit = max_register_value / cycle_rate_per_unit
 **/
uint32_t
__time_clamp_200MHz__(uint32_t intime, timetype_e ttype);

/**
 * @brief   Resolve time for 24MHz clocks
 * @details Resolve time the proper ldvalue based on the
 *          time-type of the timer manager.
 *
 * @param   gptimer_mgr -General Timer Manager
 * @return  uint32_t    -Resolved time (to:24MHz)
 *
 * @note    magic numbers calculated by incorrectly by hand
 * @todo    Find and fix the calculation mistake(s)
 */
uint32_t
__resolve_time_24MHz__(timer_manager_t * gptimer_mgr);

/**
 * @brief   Resolve time for 50MHz clocks
 * @details Resolve time the proper ldvalue based on the
 *          time-type of the timer manager.
 *
 * @param   gptimer_mgr -General Timer Manager
 * @return  uint32_t    -Resolved time (to:50MHz)
 *
 * @note    See todo on function __resolve_time_24MHz__
 */
uint32_t
__resolve_time_50MHz__(timer_manager_t * timer_mgr);

/**
 * @brief   Resolve time for 100MHz clocks
 * @details Resolve time the proper ldvalue based on the
 *          time-type of the timer manager.
 *
 * @param   gptimer_mgr -General Timer Manager
 * @return  uint32_t    -Resolved time (to:100MHz)
 *
 * @note    See todo on function __resolve_time_24MHz__
 */
uint32_t
__resolve_time_100MHz__(timer_manager_t * timer_mgr);

/**
 * @brief   Resolve time for 150MHz clocks
 * @details Resolve time the proper ldvalue based on the
 *          time-type of the timer manager.
 *
 * @param   gptimer_mgr -General Timer Manager
 * @return  uint32_t    -Resolved time (to:150MHz)
 *
 * @note    See todo on function __resolve_time_24MHz__
 */
uint32_t
__resolve_time_150MHz__(timer_manager_t * timer_mgr);

/**
 * @brief   Resolve time for 200MHz clocks
 * @details Resolve time the proper ldvalue based on the
 *          time-type of the timer manager.
 *
 * @param   gptimer_mgr -General Timer Manager
 * @return  uint32_t    -Resolved time (to:200MHz)
 *
 * @note    See todo on function __resolve_time_24MHz__
 */
uint32_t
__resolve_time_200MHz__(timer_manager_t * timer_mgr);

#endif // BASE_TIMER_H

// General Purpose Timers
#ifndef GP_TIMER_H
#define GP_TIMER_H

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
} pit_speed_e; // Speed Field

// General
#define GPT_IR_SET_EN(IR, x)      (IR & ~0x1f) | ((x)&0x1f)
#define GPT_CR_SET_EN(CR, x)      (CR & ~0x1) | ((x)&0x1)
#define GPT_CR_SET_OM1(CR, x)     (CR & ~(0x7 << 0x14)) | (((x)&0x7) << 0x14)
#define GPT_CR_SET_OM2(CR, x)     (CR & ~(0x7 << 0x17)) | (((x)&0x7) << 0x17)
#define GPT_CR_SET_OM3(CR, x)     (CR & ~(0x7 << 0x1a)) | (((x)&0x7) << 0x1a)
#define GPT_CR_IM_CLR             ~(0xf << 0x10) // [19,16]
#define GPT_CR_IM1_SET(CR, x)     ~(0x3 << 0x10) | (((x)&0x3) << 0x10) // [17,16]
#define GPT_CR_IM2_SET(CR, x)     ~(0x3 << 0x12) | (((x)&0x3) << 0x12) // [19,18]
#define GPT_CR_CLKSRC(CR, SRC)    (CR & ~(0x7 << 0x6)) | ((SRC & 0x7) << 0x6)
#define GPT_CR_SWR(CR, x)         (CR & ~(0x1 << 0xf)) | (((x)&0x1) << 0xf)
#define GPT_CR_SET_ENMOD(CR, x)   (CR & ~(0x1 << 0x1)) | (((x)&0x1) << 0x1)
#define GPT_SR_CLR                ~0x1f
#define GPT_CR_MODE(gpt_run_mode) ~(0x1 << 0x9) | ((gpt_run_mode & 0x1) << 0x9)

// GPT1 macros
#define GPT1_IR_EN(x)        GPT1_IR = GPT_IR_SET_EN(GPT1_IR, x)
#define GPT1_CR_EN(x)        GPT1_CR = GPT_CR_SET_EN(GPT1_CR, x)
#define GPT1_CR_SET_OM1(x)   GPT1_CR = GPT_CR_SET_OM1(GPT1_CR, x)
#define GPT1_CR_SET_OM2(x)   GPT1_CR = GPT_CR_SET_OM2(GPT1_CR, x)
#define GPT1_CR_SET_OM3(x)   GPT1_CR = GPT_CR_SET_OM3(GPT1_CR, x)
#define GPT1_CR_IM_CLR       GPT1_CR &= GPT_CR_IM_CLR // clear [19,16]
#define GPT1_CR_CLKSRC(SRC)  GPT1_CR = GPT_CR_CLKSRC(GPT1_CR, SRC)
#define GPT1_CR_SWR(x)       GPT1_CR = GPT_CR_SWR(GPT1_CR, x)
#define GPT1_CR_SET_ENMOD(x) GPT1_CR = GPT_CR_SET_ENMOD(GPT1_CR, x)
#define GPT1_SR_CLR          GPT1_SR &= GPT_SR_CLR

// GPT2 macros
#define GPT2_IR_EN(x)        GPT2_IR = GPT_IR_SET_EN(GPT2_IR, x)
#define GPT2_CR_EN(x)        GPT2_CR = GPT_CR_SET_EN(GPT2_CR, x)
#define GPT2_CR_SET_OM1(x)   GPT2_CR = GPT_CR_SET_OM1(GPT2_CR, x)
#define GPT2_CR_SET_OM2(x)   GPT2_CR = GPT_CR_SET_OM2(GPT2_CR, x)
#define GPT2_CR_SET_OM3(x)   GPT2_CR = GPT_CR_SET_OM3(GPT2_CR, x)
#define GPT2_CR_IM_CLR       GPT2_CR &= GPT_CR_IM_CLR // clear [19,16]
#define GPT2_CR_CLKSRC(SRC)  GPT2_CR = GPT_CR_CLKSRC(GPT2_CR, SRC)
#define GPT2_CR_SWR(x)       GPT2_CR = GPT_CR_SWR(GPT2_CR, x)
#define GPT2_CR_SET_ENMOD(x) GPT2_CR = GPT_CR_SET_ENMOD(GPT2_CR, x)
#define GPT2_SR_CLR          GPT2_SR &= GPT_SR_CLR

/**
 * @brief Clock Gating Register enum
 *
 * CGR value | Clock Activity Description
 * @enum CLK_OFF__ALL_MODES Clock is off during all modes.
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
  GPT2_E
} gptx_e;

typedef enum
{
  OCR_CH1,
  OCR_CH2,
  OCR_CH3
} gpt_ocr_e;
typedef uint32_t gpt_ocr_t;

/**
 * @todo Turn specific members into void* context memmbers,
 * and cast to appropriate context when needed
 **/
#define CONTEXT_TO_GPT(x) ((gpt_context_t *)(x))
typedef struct {
  gptx_e      gpt_x;
  gpt_ocr_e   ocr_ch;
  clk_src_e   gpt_clk;
  pit_speed_e speedfield;
} gpt_context_t;

extern timer_manager_t
    glob_gptman[6]; /** idx [0,2] == gpt1_OCRidx, idx [3,5] == gpt2_OCRidx, */

extern vuint32_t *
    glob_gpt_ptrs[6]; /** idx [0,2] == gpt1_OCRidx, idx [3,5] == gpt2_OCRidx */

void
init_gptman(timer_manager_t * gptman,
            gptx_e            gpt_x,
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

void
__set_comparator_gpt__(timer_manager_t * timer);

void
set_time(timer_manager_t * timer, timetype_e time_type, gpt_ocr_t compareval);

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
} pit_freeze_e;

// PIT Module Control Register (MCR)
typedef enum
{
  MCR_RESET = 0x0
} pit_mcr_e;

#define PIT_EN(x)      ~(0x1 << 0x1) | ((x & 0x1) << 0x1)
#define PIT_FREEZE(x)  ~0x1 | (x & 0x1)
#define PIT_MDIS_OFF   0b10
#define PIT_MDIS_ON    0b00
#define PIT_FRZ_OFF    0b01
#define PIT_FRZ_ON     0b00
#define PIT_MCR_SET(x) PIT_MCR = (PIT_MCR & ~0x3) | (x)

/**
 * @brief Shows value of timer 1.
 * @details If timer 1 is read at t1 it shows value of timer 0
 **/
#define PIT_HIGH_TIMER_READ PIT_LTMR64H

/**
 * @brief Shows value of timer 0 at the time LTMR64H was read.
 * @details Thus will only update if LTMR64H is read
 **/
#define PIT_LOW_TIMER_READ PIT_LTMR64L

/**
 * @brief Timer Load Value Register (LDVAL0 - LDVAL3), ch. 53.9.5, p 2982
 * @details These registers select the timeout period for the timer interrupts.
 **/
#define PIT_LOADVAL0_SET(x) PIT_LDVAL0 = (x)&0xffffffff
#define PIT_LOADVAL1_SET(x) PIT_LDVAL1 = (x)&0xffffffff
#define PIT_LOADVAL2_SET(x) PIT_LDVAL2 = (x)&0xffffffff
#define PIT_LOADVAL3_SET(x) PIT_LDVAL3 = (x)&0xffffffff

/**
 * @brief Current Timer Value Register (CVAL0 - CVAL3), ch. 53.9.5, p 2982
 * @details These registers indicate the current timer position.
 **/
#define PIT_CURVAL0_READ PIT_CVAL0
#define PIT_CURVAL1_READ PIT_CVAL1
#define PIT_CURVAL2_READ PIT_CVAL2
#define PIT_CURVAL3_READ PIT_CVAL3

typedef enum
{
  PIT_NOT_CHAINED = 0x0,
  PIT_CHAINED = 0x1
} pit_chainmode_e;

typedef enum
{
  PIT_TIMER_N_IRQ_DISABLED = 0x0,
  PIT_IRQ_ON_TIF = 0x1
} pit_irq_enable_e;

typedef enum
{
  PIT_TIMER_00 = 0x0,
  PIT_TIMER_01 = 0x1,
  PIT_TIMER_02 = 0x2,
  PIT_TIMER_03 = 0x3,
} pit_timer_e;

typedef enum
{
  PIT_CH1,
  PIT_CH2,
  PIT_CH3,
  PIT_CH4
} pit_ch_e;

#define CONTEXT_TO_PIT(x) ((pit_context_t *)(x))
typedef struct {
  pit_ch_e    pit_ch;
  pit_speed_e speedfield;
} pit_context_t;

/**
 * @brief   Chain Mode
 * @details When activated, timer n-1 needs to expire before timer n
 *          (n is > 0) can decrement by 1.
 *
 *          Timer 0 cannot be chained.
 *          0b - Timer is not chained.
 *          1b - Timer is chained to a previous timer.
 *          For example, for channel 2, if this field is set:
 *          Timer 2 is chained to Timer 1
 **/
#define PIT_TCTRL_CHAIN(x, MPIT_CTRL)                                          \
  (MPIT_CTRL & ~(0x1 << 0x2)) | ((x & 0x1) << 0x2)

/**
 * @brief   Timer Interrupt Enable
 * @details When an interrupt is pending, or if TFLGn[TIF] is set,
 *          enabling the interrupt causes an interrupt event.
 *
 *          @todo To avoid this, the associated TFLGn[TIF] must be cleared first.
 *          0b - Interrupt requests from Timer n are disabled.
 *          1b - Interrupt is requested whenever TIF is set.
 **/
#define PIT_TCTRL_IRQ(x, MPIT_CTRL)                                            \
  (MPIT_CTRL & ~(0x1 << 0x1)) | ((x & 0x1) << 0x1)

/**
 * @brief   Timer Enable
 * @details Enables or disables the timer.
 *          0b - Timer n is disabled.
 *          1b - Timer n is enabled
 **/
#define PIT_TCTRL_TIMER(x, MPIT_CTRL) (MPIT_CTRL & ~0x1) | (x & 0x1)

/**
 * @brief   Set Timer Control Register (TCTRL0 - TCTRL3). Ch.53.9.7  p. 2984
 *          bits [31, 3] == Reserved
 *          bits [2] == Chain Mode
 *          bits [1] == Timer Interrupt Enable
 *          bits [0] == Timer Enable
 **/
#define PIT_TCTRL0_SET(x) PIT_TCTRL0 = (x)
#define PIT_TCTRL1_SET(x) PIT_TCTRL1 = (x)
#define PIT_TCTRL2_SET(x) PIT_TCTRL2 = (x)
#define PIT_TCTRL3_SET(x) PIT_TCTRL3 = (x)

#define PIT_TCTRL0_CHAIN_SET(x) PIT_TCTRL0 = PIT_TCTRL_CHAIN(x, PIT_TCTRL0)
#define PIT_TCTRL0_IRQ_EN(x)    PIT_TCTRL0 = PIT_TCTRL_IRQ(x, PIT_TCTRL0)
#define PIT_TCTRL0_TIMER_EN(x)  PIT_TCTRL0 = PIT_TCTRL_TIMER(x, PIT_TCTRL0)

#define PIT_TCTRL1_CHAIN_SET(x) PIT_TCTRL1 = PIT_TCTRL_CHAIN(x, PIT_TCTRL3)
#define PIT_TCTRL1_IRQ_EN(x)    PIT_TCTRL1 = PIT_TCTRL_IRQ(x, PIT_TCTRL3)
#define PIT_TCTRL1_TIMER_EN(x)  PIT_TCTRL1 = PIT_TCTRL_TIMER(x, PIT_TCTRL3)

#define PIT_TCTRL2_CHAIN_SET(x) PIT_TCTRL2 = PIT_TCTRL_CHAIN(x, PIT_TCTRL3)
#define PIT_TCTRL2_IRQ_EN(x)    PIT_TCTRL2 = PIT_TCTRL_IRQ(x, PIT_TCTRL3)
#define PIT_TCTRL2_TIMER_EN(x)  PIT_TCTRL2 = PIT_TCTRL_TIMER(x, PIT_TCTRL3)

#define PIT_TCTRL3_CHAIN_SET(x) PIT_TCTRL3 = PIT_TCTRL_CHAIN(x, PIT_TCTRL3)
#define PIT_TCTRL3_IRQ_EN(x)    PIT_TCTRL3 = PIT_TCTRL_IRQ(x, PIT_TCTRL3)
#define PIT_TCTRL3_TIMER_EN(x)  PIT_TCTRL3 = PIT_TCTRL_TIMER(x, PIT_TCTRL3)

/**
 * @brief   Timer Interrupt Flag
 * @details Sets to 1 at the end of the timer period.
 *          Writing 1 to this flag clears it and writing 0 has no effect.
 *          If enabled, or, when TCTRLn[TIE] = 1, TIF causes an interrupt
 * request. 0b - Timeout has not yet occurred. 1b - Timeout has occurred.
 */
#define PIT_TFLG(x)   ~0x1 | (x & 0x1)
#define PIT_TFLG0_CLR PIT_TFLG0 &= PIT_TFLG(0x1)
#define PIT_TFLG1_CLR PIT_TFLG1 &= PIT_TFLG(0x1)
#define PIT_TFLG2_CLR PIT_TFLG2 &= PIT_TFLG(0x1)
#define PIT_TFLG3_CLR PIT_TFLG3 &= PIT_TFLG(0x1)

/**
 * @brief   in-scope construct of timer_manager-t
 * @details Construct timer manager in place on the stack
 **/
inline static timer_manager_t
construct_timer_manager_stack(gptx_e           gpt_x,
                              gpt_ocr_e        ocr_ch,
                              timetype_e       time_type,
                              timer_s          time_container,
                              clk_src_e        gpt_clk,
                              uint32_t         compval,
                              pit_speed_e      speedfield,
                              timer_manager_cb callback)
{
  /** @todo timer stack construct */
  return (timer_manager_t){.time_type = 0};
}

/**
 * @brief   user-managed construct of timer_manager-t
 * @details Construct timer manager in place on the heap
 * @return  timer_manager_t constructed on the heap
 * @note    It is user responsibiliy to free it afterwards.
 **/
inline static timer_manager_t *
construct_timer_manager_heap(gptx_e           gpt_x,
                             gpt_ocr_e        ocr_ch,
                             timetype_e       time_type,
                             timer_s          time_container,
                             clk_src_e        gpt_clk,
                             uint32_t         compval,
                             pit_speed_e      speedfield,
                             timer_manager_cb callback)
{
  /** @todo timer heap construct */
  timer_manager_t * tobj = (timer_manager_t *)malloc_(sizeof(timer_manager_t));
  return tobj;
}

/**
 * @brief   Sets values to a (pit) timer manager
 * @details Sets up a pit time based on the inputted pit_mgr
 * @param   pit_mgr Type: timer_manager_t
 * @note    Will need to set the appropriate members of pit_mgr
 *          before calling this function
 **/
void
init_pitman(timer_manager_t * pitman,
            pit_speed_e       speedfield,
            pit_ch_e          pit_ch,
            clk_src_e         pit_clk,
            timetype_e        time_type,
            timer_s           time_container,
            uint32_t          ldval,
            timer_manager_cb  callback);

/**
 * @brief   Sets appropriate PIT timer registers
 * @details
 * @param   pit_mgr  See @timer_manager_t
 *
 * @note    Do not use with uninit. pit_mgr, will cause a crash, no safety checks
 */
void
setupPITx(timer_manager_t * pit_mgr);

/**
 * @brief   Setup a Pit Timer
 * @details Sets up a pit time based on the inputted pit_mgr
 * @param   pit_mgr Type: timer_manager_t
 * @note    Will need to set the appropriate members of pit_mgr
 *          before calling this function
 **/
void
setup_pit_timer(timer_manager_t * pit_mgr, pit_timer_e pit_timerx);
void
reset_pit(timer_manager_t * pit_mgr, pit_timer_e pit_timerx);

#endif // PIT_TIMER_H