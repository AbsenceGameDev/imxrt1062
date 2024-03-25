/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2024, MIT-License included in project toplevel dir
 */

#ifndef MTIMER_MGR_H
#define MTIMER_MGR_H

/** @note 
 * As I am basing all my calculations on a 1MHz baseline, 
 * if the LDVAL needed to be halved instead or doubled when the MHz is 
 * then I should have been much more off on teh 50MHz already, 
 * but currently that is only slightly off, 
 * by a half a second to 1 second per second 
 **/


/** @todo Move these macros into a seperate macro helper header */

// [0, 4294967294] (value range)
// Total max s  if at 24MHz:  4294967294        / 24(MHz)  * 1000000 = 4294967294    / 24000000  =           178 (Fraction =) 0,9569705833333
// Total max ms if at 24MHz:  4294967294        / 24(MHz)  *    1000 = 4294967294    / 24000     =        178956 (Fraction =) 0,9705833333
// Total max us if at 24MHz:  4294967294        / 24(MHz)  *       1 = 4294967294    / 24        =     178956970 (Fraction =) 0,5833333 
// Total max zs if at 24MHz:  4294967294 * 10   / 24(MHz)  *       1 = 42949672940   / 24        =    1789569705 (Fraction =) 0,833333
// Total max ys if at 24MHz:  4294967294 * 100  / 24(MHz)  *       1 = 429496729400  / 24        =   17895697058 (Fraction =) 0,333333
// Total max ns if at 24MHz:  4294967294 * 1000 / 24(MHz)  *       1 = 4294967294000 / 24        =  178956970583 (Fraction =) 0,3333
#define TRANSLATE_TIME_24MHz(time_og_unit, timetype)           \
  (time_og_unit  * 1440000000UL)   * (timetype == MINUTES_E) + \
  (time_og_unit  * 24000000UL)     * (timetype == SECONDS_E) + \
  (time_og_unit  * 24000UL)        * (timetype == MILLIS_E)  + \
  (time_og_unit  * 24UL)           * (timetype == MICROS_E)  + \
  ((time_og_unit * 24UL) / 10UL)   * (timetype == ZETTOS_E)  + \
  ((time_og_unit * 24UL) / 100UL)  * (timetype == YOTTOS_E)  + \
  ((time_og_unit * 24UL) / 1000UL) * (timetype == NANOS_E)
#define BACKTRANSLATE_TIME_24MHz(time_og_unit, timetype)           \
  (time_og_unit  / 1440000000UL)   * (timetype == MINUTES_E) + \
  (time_og_unit  / 24000000UL)     * (timetype == SECONDS_E) + \
  (time_og_unit  / 24000UL)        * (timetype == MILLIS_E)  + \
  (time_og_unit  / 24UL)           * (timetype == MICROS_E)  + \
  ((time_og_unit * 10UL) / 24UL)   * (timetype == ZETTOS_E)  + \
  ((time_og_unit * 100UL) / 24UL)  * (timetype == YOTTOS_E)  + \
  ((time_og_unit * 1000UL) / 24UL) * (timetype == NANOS_E)


// Total max s  if at 50MHz:  4294967294        / 50(MHz)  * 1000000 = 4294967294    / 50000000  =           85 (Fraction =) 0,89934588
// Total max ms if at 50MHz:  4294967294        / 50(MHz)  *    1000 = 4294967294    / 50000     =        85899 (Fraction =) 0,34588 
// Total max us if at 50MHz:  4294967294        / 50(MHz)  *       1 = 4294967294    / 50        =     85899345 (Fraction =) 0,88
// Total max zs if at 50MHz:  4294967294 * 10   / 50(MHz)  *       1 = 42949672940   / 50        =    858993458 (Fraction =) 0,8
// Total max ys if at 50MHz:  4294967294 * 100  / 50(MHz)  *       1 = 429496729400  / 50        =   8589934588 (Fraction =) 0,0
// Total max ns if at 50MHz:  4294967294 * 1000 / 50(MHz)  *       1 = 4294967294000 / 50        =  85899345880 (Fraction =) 0,0
#define TRANSLATE_TIME_50MHz(time_og_unit, timetype)         \
  (time_og_unit  * 3000000000UL) * (timetype == MINUTES_E) + \
  (time_og_unit  * 50000000UL)   * (timetype == SECONDS_E) + \
  (time_og_unit  * 50000UL)      * (timetype == MILLIS_E)  + /* 25,000,000 register increments, if time_og_unit was 500 millis */ \
  (time_og_unit  * 50UL)         * (timetype == MICROS_E)  + \
  (time_og_unit  * 5UL)          * (timetype == ZETTOS_E)  + \
  ((time_og_unit * 5UL) / 10UL)  * (timetype == YOTTOS_E)  + \
  ((time_og_unit * 5UL) / 100UL) * (timetype == NANOS_E)

#define BACKTRANSLATE_TIME_50MHz(time_translated_unit, timetype)         \
  (time_translated_unit  / 3000000000UL) * (timetype == MINUTES_E) + \
  (time_translated_unit  / 50000000UL)   * (timetype == SECONDS_E) + \
  (time_translated_unit  / 50000UL)      * (timetype == MILLIS_E)  + /* 25,000,000 register increments, if time_og_unit was 500 millis */ \
  (time_translated_unit  / 50UL)         * (timetype == MICROS_E)  + \
  (time_translated_unit  / 5UL)          * (timetype == ZETTOS_E)  + \
  ((time_translated_unit * 10UL) / 5UL)  * (timetype == YOTTOS_E)  + \
  ((time_translated_unit * 100UL) / 5UL) * (timetype == NANOS_E)  

  /*BEFORE: 25000000U*/
  /*BEFORE: 6250U*/
  /*BEFORE: ((time_og_unit * 6250U) / 80U)*/
  /*BEFORE: ((time_og_unit * 6250U) / 80U)*/
  /*BEFORE: ((time_og_unit * 50U)   / 100U)*/
  /*BEFORE: ((time_og_unit * 50U)   / 1000U)*/

// If putting 1 second into the 100MHz clamp funciton, it returns 1 as expected, whta happens below in that case? 
// This should be right if 100MHz indeed means 100Mega ticks of the register/counter per second

// Total max s  if at 100MHz: 4294967294        / 100(MHz) * 1000000 = 4294967294    / 100000000  =           42 (Fraction =) 0,94967294
// Total max ms if at 100MHz: 4294967294        / 100(MHz) *    1000 = 4294967294    / 100000     =        42949 (Fraction =) 0,67294
// Total max us if at 100MHz: 4294967294        / 100(MHz) *       1 = 4294967294    / 100        =     42949672 (Fraction =) 0,94
// Total max zs if at 100MHz: 4294967294 * 10   / 100(MHz) *       1 = 42949672940   / 100        =    429496729 (Fraction =) 0,4
// Total max ys if at 100MHz: 4294967294 * 100  / 100(MHz) *       1 = 429496729400  / 100        =   4294967294 (Fraction =) 0,0
// Total max ns if at 100MHz: 4294967294 * 1000 / 100(MHz) *       1 = 4294967294000 / 100        =  42949672940 (Fraction =) 0,0
#define TRANSLATE_TIME_100MHz(time_og_unit, timetype)       \
  (time_og_unit  * 100000000UL) * (timetype == SECONDS_E) + \
  (time_og_unit  * 100000UL)    * (timetype == MILLIS_E)  + \
  (time_og_unit  * 100UL)       * (timetype == MICROS_E)  + \
  (time_og_unit  * 10UL)        * (timetype == ZETTOS_E)  + \
  (time_og_unit  * 1UL)         * (timetype == YOTTOS_E)  + \
  ((time_og_unit * 1UL) / 10UL) * (timetype == NANOS_E)
#define BACKTRANSLATE_TIME_100MHz(time_og_unit, timetype)       \
  (time_og_unit  / 100000000UL) * (timetype == SECONDS_E) + \
  (time_og_unit  / 100000UL)    * (timetype == MILLIS_E)  + \
  (time_og_unit  / 100UL)       * (timetype == MICROS_E)  + \
  (time_og_unit  / 10UL)        * (timetype == ZETTOS_E)  + \
  (time_og_unit)                * (timetype == YOTTOS_E)  + \
  (time_og_unit  * 10UL)        * (timetype == NANOS_E)


#define TRANSLATE_TIME_150MHz(time_og_unit, timetype)         \
  (time_og_unit  * 150000000UL)   * (timetype == SECONDS_E) + \
  (time_og_unit  * 150000UL)      * (timetype == MILLIS_E)  + \
  (time_og_unit  * 150UL)         * (timetype == MICROS_E)  + \
  (time_og_unit  * 15UL)          * (timetype == ZETTOS_E)  + \
  ((time_og_unit * 15UL) / 10UL)  * (timetype == YOTTOS_E)  + \
  ((time_og_unit * 15UL) / 100UL) * (timetype == NANOS_E)
#define BACKTRANSLATE_TIME_150MHz(time_og_unit, timetype)         \
  (time_og_unit  / 150000000UL)   * (timetype == SECONDS_E) + \
  (time_og_unit  / 150000UL)      * (timetype == MILLIS_E)  + \
  (time_og_unit  / 150UL)         * (timetype == MICROS_E)  + \
  (time_og_unit  / 15UL)          * (timetype == ZETTOS_E)  + \
  ((time_og_unit * 10UL) / 15UL)  * (timetype == YOTTOS_E)  + \
  ((time_og_unit * 100UL) / 15UL) * (timetype == NANOS_E)


#define TRANSLATE_TIME_200MHz(time_og_unit, timetype)       \
  (time_og_unit  * 200000000UL) * (timetype == SECONDS_E) + \
  (time_og_unit  * 200000UL)    * (timetype == MILLIS_E)  + \
  (time_og_unit  * 200UL)       * (timetype == MICROS_E)  + \
  (time_og_unit  * 20UL)        * (timetype == ZETTOS_E)  + \
  (time_og_unit  * 2UL)         * (timetype == YOTTOS_E)  + \
  ((time_og_unit * 2UL) / 10UL) * (timetype == NANOS_E)
#define BACKTRANSLATE_TIME_200MHz(time_og_unit, timetype)       \
  (time_og_unit  / 200000000UL) * (timetype == SECONDS_E) + \
  (time_og_unit  / 200000UL)    * (timetype == MILLIS_E)  + \
  (time_og_unit  / 200UL)       * (timetype == MICROS_E)  + \
  (time_og_unit  / 20UL)        * (timetype == ZETTOS_E)  + \
  (time_og_unit  / 2UL)         * (timetype == YOTTOS_E)  + \
  ((time_og_unit * 10UL) / 2UL) * (timetype == NANOS_E)


/* TRANSLATE CLAMPS */

// Total max s  if at 24MHz:  4294967294        / 24(MHz)  * 1000000 = 4294967294    / 24000000  =           178 (Fraction =) 0,9569705833333
// Total max ms if at 24MHz:  4294967294        / 24(MHz)  *    1000 = 4294967294    / 24000     =        178956 (Fraction =) 0,9705833333
// Total max us if at 24MHz:  4294967294        / 24(MHz)  *       1 = 4294967294    / 24        =     178956970 (Fraction =) 0,5833333 
// Total max zs if at 24MHz:  4294967294 * 10   / 24(MHz)  *       1 = 42949672940   / 24        =    1789569705 (Fraction =) 0,833333
// Total max ys if at 24MHz:  4294967294 * 100  / 24(MHz)  *       1 = 429496729400  / 24        =   17895697058 (Fraction =) 0,333333
// Total max ns if at 24MHz:  4294967294 * 1000 / 24(MHz)  *       1 = 4294967294000 / 24        =  178956970583 (Fraction =) 0,3333
#define TRANSLATE_TIMECLAMP_24MHz(intime, timetype)                \
  2U          * (timetype == MINUTES_E && intime >           2UL) + \
  178U        * (timetype == SECONDS_E && intime >         178UL) + \
  178956U     * (timetype == MILLIS_E  && intime >      178956UL) + \
  178956970U  * (timetype == MICROS_E  && intime >   178956970UL) + \
  1789569706U * (timetype == ZETTOS_E  && intime >  1789569706UL) + \
  0xffffffff  * (timetype == YOTTOS_E  && intime >=  0xffffffff)  + \
  0xffffffff  * (timetype == NANOS_E   && intime >=  0xffffffff)

// Total max s  if at 50MHz:  4294967294        / 50(MHz)  * 1000000 = 4294967294    / 50000000  =           85 (Fraction =) 0,89934588
// Total max ms if at 50MHz:  4294967294        / 50(MHz)  *    1000 = 4294967294    / 50000     =        85899 (Fraction =) 0,34588 
// Total max us if at 50MHz:  4294967294        / 50(MHz)  *       1 = 4294967294    / 50        =     85899345 (Fraction =) 0,88
// Total max zs if at 50MHz:  4294967294 * 10   / 50(MHz)  *       1 = 42949672940   / 50        =    858993458 (Fraction =) 0,8
// Total max ys if at 50MHz:  4294967294 * 100  / 50(MHz)  *       1 = 429496729400  / 50        =   8589934588 (Fraction =) 0,0
// Total max ns if at 50MHz:  4294967294 * 1000 / 50(MHz)  *       1 = 4294967294000 / 50        =  85899345880 (Fraction =) 0,0
#define TRANSLATE_TIMECLAMP_50MHz(intime, timetype)              \
  1U         * (timetype == MINUTES_E && intime >          1U) + \
  85U        * (timetype == SECONDS_E && intime >         85U) + \
  85899U     * (timetype == MILLIS_E  && intime >      85899U) + \
  85899345U  * (timetype == MICROS_E  && intime >   85899345U) + \
  858993459U * (timetype == ZETTOS_E  && intime >  858993459U) + \
  0xffffffff * (timetype == YOTTOS_E  && intime >= 0xffffffff) + \
  0xffffffff * (timetype == NANOS_E   && intime >= 0xffffffff)

// Total max s  if at 100MHz: 4294967294        / 100(MHz) * 1000000 = 4294967294    / 100000000  =           42 (Fraction =) 0,94967294
// Total max ms if at 100MHz: 4294967294        / 100(MHz) *    1000 = 4294967294    / 100000     =        42949 (Fraction =) 0,67294
// Total max us if at 100MHz: 4294967294        / 100(MHz) *       1 = 4294967294    / 100        =     42949672 (Fraction =) 0,94
// Total max zs if at 100MHz: 4294967294 * 10   / 100(MHz) *       1 = 42949672940   / 100        =    429496729 (Fraction =) 0,4
// Total max ys if at 100MHz: 4294967294 * 100  / 100(MHz) *       1 = 429496729400  / 100        =   4294967294 (Fraction =) 0,0
// Total max ns if at 100MHz: 4294967294 * 1000 / 100(MHz) *       1 = 4294967294000 / 100        =  42949672940 (Fraction =) 0,0
#define TRANSLATE_TIMECLAMP_100MHz(intime, timetype)             \
  42U        * (timetype == SECONDS_E && intime >         42U) + \
  42949U     * (timetype == MILLIS_E  && intime >      42949U) + \
  42949672U  * (timetype == MICROS_E  && intime >   42949672U) + \
  429496729U * (timetype == ZETTOS_E  && intime >  429496729U) + \
  0xffffffff * (timetype == YOTTOS_E  && intime >= 0xffffffff) + \
  0xffffffff * (timetype == NANOS_E   && intime >= 0xffffffff)

// Total max s  if at 150MHz: 4294967294        / 150(MHz) * 1000000 = 4294967294    / 150000000  =          14 (Fraction =) 0,31655764666667
// Total max ms if at 150MHz: 4294967294        / 150(MHz) * 1000    = 4294967294    / 150000     =       28633 (Fraction =) 0,11529333333
// Total max us if at 150MHz: 4294967294        / 150(MHz) * 1       = 4294967294    / 150        =    28633115 (Fraction =) 0,29333333
// Total max zs if at 150MHz: 4294967294 * 10   / 150(MHz) * 1       = 42949672940   / 150        =   286331152 (Fraction =) 0,9333333
// Total max ys if at 150MHz: 4294967294 * 100  / 150(MHz) * 1       = 429496729400  / 150        =  2863311529 (Fraction =) 0,333333  
// Total max ns if at 150MHz: 4294967294 * 1000 / 150(MHz) * 1       = 4294967294000 / 150        = 28633115293 (Fraction =) 0,33333 
#define TRANSLATE_TIMECLAMP_150MHz(intime, timetype)                \
  14U         * (timetype == SECONDS_E && intime >          14U) +  \
  14316U      * (timetype == MILLIS_E  && intime >       14316U) +  \
  14316557U   * (timetype == MICROS_E  && intime >    14316557U) +  \
  286331153U  * (timetype == ZETTOS_E  && intime >   286331153U) +  \
  2863311530U * (timetype == YOTTOS_E  && intime >= 2863311530U) +  \
  0xffffffff  * (timetype == NANOS_E   && intime >=  0xffffffff)


// Total max s  if at 200MHz: 4294967294        / 200(MHz) * 1000000 = 4294967294    / 200000000 =          21 (Fraction =) 0,47483647
// Total max ms if at 200MHz: 4294967294        / 200(MHz) * 1000    = 4294967294    / 200000    =       21474 (Fraction =) 0,83647
// Total max us if at 200MHz: 4294967294        / 200(MHz) * 1       = 4294967294    / 200       =    21474836 (Fraction =) 0,47
// Total max zs if at 200MHz: 4294967294 * 10   / 200(MHz) * 1       = 42949672940   / 200       =   214748364 (Fraction =) 0,7
// Total max ys if at 200MHz: 4294967294 * 100  / 200(MHz) * 1       = 429496729400  / 200       =  2147483647 (Fraction =) 0,0
// Total max ns if at 200MHz: 4294967294 * 1000 / 200(MHz) * 1       = 4294967294000 / 200       = 21474836470 (Fraction =) 0,0
#define TRANSLATE_TIMECLAMP_200MHz(intime, timetype)                  \
  21UL         * (timetype == SECONDS_E && intime >          21UL) +  \
  21474UL      * (timetype == MILLIS_E  && intime >       21474UL) +  \
  21474836UL   * (timetype == MICROS_E  && intime >    21474836UL) +  \
  214748364UL  * (timetype == ZETTOS_E  && intime >   214748364UL) +  \
  2147483647UL * (timetype == YOTTOS_E  && intime >= 2147483647UL) +  \
  0xffffffff   * (timetype == NANOS_E   && intime >=   0xffffffff)

#endif // MTIMER_MGR_H