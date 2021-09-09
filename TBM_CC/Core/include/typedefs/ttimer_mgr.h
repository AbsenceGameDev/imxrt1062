#ifndef T_TIMER_MGR_H
#define T_TIMER_MGR_H

#include <stdint.h>

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

#endif // T_TIMER_MGR_H