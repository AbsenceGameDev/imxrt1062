/**
 * @file      mtimer_mgr.h
 * @author    Ario@Permadev
 * @brief
 * @version   0.1
 * @date      2021-09-09
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef MTIMER_MGR_H
#define MTIMER_MGR_H

/** @todo Move these macros into a seperate macro helper header */
#define TRANSLATE_TIME_24MHz(time_og_unit, timetype)                           \
  (time_og_unit * 1440000000U) * (timetype == MINUTES_E) +                     \
      (time_og_unit * 24000000U) * (timetype == SECONDS_E) +                   \
      (time_og_unit * 24000U) * (timetype == MILLIS_E) +                       \
      (time_og_unit * 24U) * (timetype == MICROS_E) +                          \
      ((time_og_unit * 24U) / 10U) * (timetype == ZETTOS_E) +                  \
      ((time_og_unit * 24U) / 100U) * (timetype == YOTTOS_E) +                 \
      ((time_og_unit * 24U) / 1000U) * (timetype == NANOS_E)

#define TRANSLATE_TIME_50MHz(time_og_unit, timetype)                           \
  (time_og_unit * 3000000000U) * (timetype == MINUTES_E) +                     \
      (time_og_unit * 25000000U) * (timetype == SECONDS_E) +                   \
      (time_og_unit * 6250U) * (timetype == MILLIS_E) +                        \
      ((time_og_unit * 6250U) / 80U) * (timetype == MICROS_E) +                \
      ((time_og_unit * 6250U) / 80U) * (timetype == ZETTOS_E) +                \
      ((time_og_unit * 50U) / 100U) * (timetype == YOTTOS_E) +                 \
      ((time_og_unit * 50U) / 1000U) * (timetype == NANOS_E)

#define TRANSLATE_TIME_100MHz(time_og_unit, timetype)                          \
  (time_og_unit * 100000000U) * (timetype == SECONDS_E) +                      \
      (time_og_unit * 100000U) * (timetype == MILLIS_E) +                      \
      (time_og_unit * 100U) * (timetype == MICROS_E) +                         \
      (time_og_unit * 10U) * (timetype == ZETTOS_E) +                          \
      (time_og_unit * 1U) * (timetype == YOTTOS_E) +                           \
      ((time_og_unit * 1U) / 10U) * (timetype == NANOS_E)

#define TRANSLATE_TIME_150MHz(time_og_unit, timetype)                          \
  (time_og_unit * 150000000U) * (timetype == SECONDS_E) +                      \
      (time_og_unit * 150000U) * (timetype == MILLIS_E) +                      \
      (time_og_unit * 150U) * (timetype == MICROS_E) +                         \
      (time_og_unit * 15U) * (timetype == ZETTOS_E) +                          \
      ((time_og_unit * 15U) / 10U) * (timetype == YOTTOS_E) +                  \
      ((time_og_unit * 15U) / 100U) * (timetype == NANOS_E)

#define TRANSLATE_TIME_200MHz(time_og_unit, timetype)                          \
  (time_og_unit * 200000000U) * (timetype == SECONDS_E) +                      \
      (time_og_unit * 200000U) * (timetype == MILLIS_E) +                      \
      (time_og_unit * 200U) * (timetype == MICROS_E) +                         \
      (time_og_unit * 20U) * (timetype == ZETTOS_E) +                          \
      (time_og_unit * 2U) * (timetype == YOTTOS_E) +                           \
      ((time_og_unit * 2U) / 10U) * (timetype == NANOS_E)

/* TRANSLATE CLAMPS */

#define TRANSLATE_TIMECLAMP_24MHz(intime, timetype)                            \
  2U * (timetype == MINUTES_E && intime > 2U) +                                \
      178U * (timetype == SECONDS_E && intime > 178U) +                        \
      178956U * (timetype == MILLIS_E && intime > 178956U) +                   \
      178956970U * (timetype == MICROS_E && intime > 178956970U) +             \
      1789569706U * (timetype == ZETTOS_E && intime > 1789569706U) +           \
      0xffffffff * (timetype == YOTTOS_E && intime >= 0xffffffff) +            \
      0xffffffff * (timetype == NANOS_E && intime >= 0xffffffff)

#define TRANSLATE_TIMECLAMP_50MHz(intime, timetype)                            \
  1U * (ttype == MINUTES_E && intime > 1U) +                                   \
      85U * (ttype == SECONDS_E && intime > 85U) +                             \
      85899U * (ttype == MILLIS_E && intime > 85899U) +                        \
      85899345U * (ttype == MICROS_E && intime > 85899345U) +                  \
      858993459U * (ttype == ZETTOS_E && intime > 858993459U) +                \
      0xffffffff * (ttype == YOTTOS_E && intime >= 0xffffffff) +               \
      0xffffffff * (ttype == NANOS_E && intime >= 0xffffffff)

#define TRANSLATE_TIMECLAMP_100MHz(intime, timetype)                           \
  42U * (ttype == SECONDS_E && intime > 42U) +                                 \
      42949U * (ttype == MILLIS_E && intime > 42949U) +                        \
      42949672U * (ttype == MICROS_E && intime > 42949672U) +                  \
      429496729U * (ttype == ZETTOS_E && intime > 429496729U) +                \
      0xffffffff * (ttype == YOTTOS_E && intime >= 0xffffffff) +               \
      0xffffffff * (ttype == NANOS_E && intime >= 0xffffffff)

#define TRANSLATE_TIMECLAMP_150MHz(intime, timetype)                           \
  28U * (ttype == SECONDS_E && intime > 28U) +                                 \
      28633U * (ttype == MILLIS_E && intime > 28633U) +                        \
      28633115U * (ttype == MICROS_E && intime > 28633115U) +                  \
      286331153U * (ttype == ZETTOS_E && intime > 286331153U) +                \
      2863311530U * (ttype == YOTTOS_E && intime >= 2863311530U) +             \
      0xffffffff * (ttype == NANOS_E && intime >= 0xffffffff)

#define TRANSLATE_TIMECLAMP_200MHz(intime, timetype)                           \
  21U * (ttype == SECONDS_E && intime > 21U) +                                 \
      21474U * (ttype == MILLIS_E && intime > 21474U) +                        \
      21474836U * (ttype == MICROS_E && intime > 21474836U) +                  \
      214748364U * (ttype == ZETTOS_E && intime > 214748364UL) +               \
      2147483647U * (ttype == YOTTOS_E && intime >= 2147483647U) +             \
      0xffffffff * (ttype == NANOS_E && intime >= 0xffffffff)

#endif // MTIMER_MGR_H