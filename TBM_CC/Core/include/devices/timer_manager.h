/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2024, MIT-License included in project toplevel dir
 */

#include "sys/irq_handler.h"
#include "sys/heap.h"
#include "sys/memory_map.h"
#include "clk_control.h"

/* Utils@Permadev */
#include "utils/utimer_mgr.h"

// General Purpose Timers
#ifndef GP_TIMER_H
  #define GP_TIMER_H

  // General
  #define GPT_IR_SET_EN(IR, x)    (IR & ~0x1f) | ((x)&0x1f)
  #define GPT_CR_SET_EN(CR, x)    (CR & ~0x1) | ((x)&0x1)
  #define GPT_CR_SET_OM1(CR, x)   (CR & ~(0x7 << 0x14)) | (((x)&0x7) << 0x14)
  #define GPT_CR_SET_OM2(CR, x)   (CR & ~(0x7 << 0x17)) | (((x)&0x7) << 0x17)
  #define GPT_CR_SET_OM3(CR, x)   (CR & ~(0x7 << 0x1a)) | (((x)&0x7) << 0x1a)
  #define GPT_CR_IM_CLR           ~(0xf << 0x10) // [19,16]
  #define GPT_CR_IM1_SET(CR, x)   ~(0x3 << 0x10) | (((x)&0x3) << 0x10) // [17,16]
  #define GPT_CR_IM2_SET(CR, x)   ~(0x3 << 0x12) | (((x)&0x3) << 0x12) // [19,18]
  #define GPT_CR_CLKSRC(CR, SRC)  (CR & ~(0x7 << 0x6)) | ((SRC & 0x7) << 0x6)
  #define GPT_CR_SWR(CR, x)       (CR & ~(0x1 << 0xf)) | (((x)&0x1) << 0xf)
  #define GPT_CR_SET_ENMOD(CR, x) (CR & ~(0x1 << 0x1)) | (((x)&0x1) << 0x1)
  #define GPT_SR_CLR              ~0x1f
  #define GPT_CR_MODE(gpt_run_mode) \
    ~(0x1 << 0x9) | ((gpt_run_mode & 0x1) << 0x9)

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

  // // Some defines to set enable GPTx at the Clock Controller Module (CCM)
  /** @brief Set gpt2 bus clock field in CCM Clock gating register 0, p.1085 */
  #define GPT2_SET_BUS(x) (uint32_t)(((x)&0b11) << 24)

  /** @brief set gpt2 bus clock field in CCM Clock gating register 0, p.1085 */
  #define GPT2_SET_SERIAL(x) (uint32_t)(((x)&0b11) << 26)
  #define CCM_C_GPT2_EN \
    CCM_C_CGR0 |=       \
        GPT2_SET_BUS(CLK_ON__NO_STOP) | GPT2_SET_SERIAL(CLK_ON__NO_STOP)

  /** @brief Set gpt1 bus clock field in CCM Clock gating register 1, p.1085 */
  #define GPT1_SET_BUS(x) (uint32_t)(((x)&0b11) << 20)

  /** @brief set gpt1 bus clock field in CCM Clock gating register 1, p.1085 */
  #define GPT1_SET_SERIAL(x) (uint32_t)(((x)&0b11) << 22)
  #define CCM_C_GPT1_EN \
    CCM_C_CGR1 |=       \
        GPT1_SET_BUS(CLK_ON__NO_STOP) | GPT1_SET_SERIAL(CLK_ON__NO_STOP)

  /**
   * @todo Turn specific members into void* context members,
   * and cast to appropriate context when needed
   **/
  #define CONTEXT_TO_GPT(x) ((gpt_context_s*)(x))

extern timer_manager_s
    glob_gptman[6]; /** idx [0,2] == gpt1_OCRidx, idx [3,5] == gpt2_OCRidx, */

extern vuint32_t*
    glob_gpt_ptrs[6]; /** idx [0,2] == gpt1_OCRidx, idx [3,5] == gpt2_OCRidx */

void
init_gptman(timer_manager_s* restrict gptman,
            gptx_e           gpt_x,
            gpt_ocr_e        ocr_ch,
            clk_src_e        gpt_clk,
            timetype_e       time_type,
            uint32_t         compval,
            timer_manager_cb interrupt_callback);

void
__slct_clksrc_gpt__(timer_manager_s* restrict gptman);

void
__set_callback_gpt__(timer_manager_s* restrict gptman,
                     timer_manager_cb interrupt_callback);

void
__set_comparator_gpt__(timer_manager_s* restrict gptman);

void
set_time(timer_manager_s* restrict gptman,
         timetype_e time_type,
         gpt_ocr_t  compval);

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

#define CONTEXT_TO_PIT(x) ((pit_context_s*)(x))

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
  #define PIT_TCTRL_CHAIN(x, MPIT_CTRL) \
    (MPIT_CTRL & ~(0x1 << 0x2)) | ((x & 0x1) << 0x2)

  /**
   * @brief   Timer Interrupt Enable
   * @details When an interrupt is pending, or if TFLGn[TIF] is set,
   *          enabling the interrupt causes an interrupt event.
   *
   *          @todo To avoid this, the associated TFLGn[TIF] must be cleared
   *first. 0b - Interrupt requests from Timer n are disabled. 1b - Interrupt is
   *requested whenever TIF is set.
   **/
  #define PIT_TCTRL_IRQ(x, MPIT_CTRL) \
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
 * @brief   user-managed construct of timer_manager-t
 * @details Construct timer manager in place on the heap
 * @return  timer_manager_s constructed on the heap
 * @note    It is user responsibility to free it afterwards.
 **/
inline static timer_manager_s*
construct_timer_manager_heap(gptx_e           gpt_x,
                             gpt_ocr_e        ocr_ch,
                             timetype_e       time_type,
                             clk_src_e        gpt_clk,
                             uint32_t         compval,
                             pit_speed_e      freq,
                             timer_manager_cb interrupt_callback)
{
  /** @todo timer heap construct */
  timer_manager_s* tobj = (timer_manager_s*)malloc_(sizeof(timer_manager_s));
  return tobj;
}

/**
 * @brief   Sets values to a (pit) timer manager
 * @details Sets up a pit time based on the inputted pit_mgr
 * @param   pit_mgr Type: timer_manager_s
 * @note    Will need to set the appropriate members of pit_mgr
 *          before calling this function
 **/
void
init_pitman(timer_manager_s* restrict pitman,
            timer_datum_s* restrict   timerdatum,
            pit_ch_e              pit_ch,
            timer_manager_cb      interrupt_callback,
            timer_manager_sick_cb tick_callback);

/**
 * @brief   Sets appropriate PIT timer registers
 * @details
 * @param   pit_mgr  See @timer_manager_s
 *
 * @note    Do not use with uninit. pit_mgr, will cause a crash, no safety checks
 */
void
setup_PITx(timer_manager_s* restrict pitman, vuint32_t* Flags, vuint32_t DirectionBit);


/**
 * @brief   Resolves the ticks from the given value and time type
 * @details
 */
uint32_t resolve_time(pit_speed_e freq, timetype_e time_type, uint32_t targetval, ttconversiondir_e conversiondir);

/**
 * @brief   Generates a struct compatible with the timer abstraction.  
 * @details Clamps the timeval in the returning 'timer_datum_s' to a range valid for the selected frequency for the selected time_type.
 */
timer_datum_s generate_time_struct(pit_speed_e freq, timetype_e type, uint32_t timeval);

/**
 * @brief   Setup a Pit Timer
 * @details Sets up a pit time based on the inputted pit_mgr
 * @param   pitman     Timer manager
 * @param   pit_timerx Timer selector 
 * @note    Must set the appropriate members of pitman before calling this function
 **/
void
setup_pit_irq(timer_manager_s* restrict pitman, pit_timer_e pit_timerx);

/**
 * @brief   Reset a Pit Timer
 * @details Clears and reenables a pit timer
 * @param   pitman     Timer manager
 * @param   pit_timerx Timer selector 
 * @note    Must set the appropriate members of pitman before calling this function
 **/
void
reset_pit(timer_manager_s* restrict pitman, pit_timer_e pit_timerx);

/** @brief  Hardware trigger tick, used for IRQs */
void hwtick();

/** @brief  Software polling timer tick, used for polling purposes when accuracy is a lesser concern */ 
void polltick(const float delta_time);

#endif // PIT_TIMER_H