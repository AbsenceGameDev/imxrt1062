/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2026, MIT-License included in project toplevel dir
 */

#ifndef T_TIMER_MGR_H
#define T_TIMER_MGR_H

#include <stdint.h>

/// GENERIC

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

typedef union 
{
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

typedef enum
{
  FROMTIME_E,
  FROMTICKS_E,
  
} ttconversiondir_e; // time-tick conversion

typedef enum
{
  TIME_SPEED_24MHz = 0x0,
  TIME_SPEED_50MHz = 0x1,
  TIME_SPEED_75MHz = 0x2,
  TIME_SPEED_100MHz = 0x3,
  TIME_SPEED_150MHz = 0x4,
  TIME_SPEED_200MHz = 0x5
} generic_speed_e; // Speed Field

typedef enum
{
  PIT_SPEED_50MHz = 0x0,
  PIT_SPEED_100MHz = 0x1,
  PIT_SPEED_150MHz = 0x2,
  PIT_SPEED_200MHz = 0x3
} pit_speed_e; // Speed Field


typedef struct 
{
  timeunit_u count;
  uint16_t   steps; // steps in this case being wrap
} timer_s;

typedef struct 
{
  pit_speed_e freq;
  timetype_e  type;
  uint32_t    val;
} timer_datum_s;

typedef void (*timer_manager_cb)(void);
typedef void (*timer_manager_sick_cb)(float);

typedef enum
{
  GPT_E,
  PIT_E,
  QUAD_E,
  WDOG_E,
  RTWDOG_E
} timer_type_e;

typedef struct 
{
  timer_type_e type;
  void*        base_gpio_device;
  void*        context;
} timer_context_s;

typedef struct 
{
  timer_manager_cb       interrupt_callback;
  timer_manager_sick_cb  tick_callback;
  timer_context_s*       timer_ctx;
  uint32_t               targetval;
  clk_src_e              clk_src;
  timetype_e             time_type;
  uint8_t                keep_ticking;
} timer_manager_s;


/// GPT

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

typedef enum
{
  GPT1_E,
  GPT2_E
} gptx_e;

/** @brief OCR Channel Selector */
typedef enum
{
  OCR_CH1,
  OCR_CH2,
  OCR_CH3
} gpt_ocr_e;
typedef uint32_t gpt_ocr_t;

/** @brief gpt context */
typedef struct 
{
  gptx_e      gpt_x;
  gpt_ocr_e   ocr_ch;
  clk_src_e   gpt_clk;
  pit_speed_e freq;
} gpt_context_s;


/// PIT

/** @brief PIT Module Control Register (MCR) flags - FRZ, Freeze/RunInDebug */
typedef enum
{
  PIT_RUN_IN_DEBUG = 0x0,
  PIT_STOP_IN_DEBUG = 0x1
} pit_freeze_e;

/** @brief PIT Module Control Register (MCR) flags - MCR Reset */
typedef enum
{
  MCR_RESET = 0x0
} pit_mcr_e;


/** @brief PIT Timer Control Register (TCTRLx) flags - Chain Mode */
typedef enum
{
  PIT_NOT_CHAINED = 0x0,
  PIT_CHAINED = 0x1
} pit_chainmode_e;

/** @brief PIT Timer Control Register (TCTRLx) flags - TIE, Timer Interrupt Enable */
typedef enum
{
  PIT_TIMER_N_IRQ_DISABLED = 0x0,
  PIT_IRQ_ON_TIF = 0x1
} pit_irq_enable_e;

/** @brief PIT Timer selector */
typedef enum
{
  PIT_TIMER_00 = 0x0,
  PIT_TIMER_01 = 0x1,
  PIT_TIMER_02 = 0x2,
  PIT_TIMER_03 = 0x3,
} pit_timer_e;

/** @brief PIT Channel selector */
typedef enum
{
  PIT_CH1,
  PIT_CH2,
  PIT_CH3,
  PIT_CH4
} pit_ch_e;


/** @brief PIT context */
typedef struct 
{
  pit_ch_e    pit_ch;
  pit_speed_e freq;
} pit_context_s;

typedef union
{
  int32_t AsInt32;
  float AsFloat;
} punned_float_u;

#endif // T_TIMER_MGR_H
