#include "timer_manager.h"

// Globals
timer_manager_t glob_gptman[6];
vuint32_t *     glob_gpt_ptrs[6] = {
    &GPT1_OCR1, &GPT1_OCR2, &GPT1_OCR3, &GPT2_OCR1, &GPT2_OCR2, &GPT2_OCR3};

void
init_gptman(timer_manager_t * gptman,
            gptx_e            gpt_x,
            gpt_ocr_e         ocr_ch,
            clk_src_e         gpt_clk,
            timetype_e        time_type,
            timer_s           time_container,
            uint32_t          compval,
            timer_manager_cb  callback)
{
  void * context = gptman->timer_ctx->context;
  CONTEXT_TO_GPT(context)->gpt_x = gpt_x;
  CONTEXT_TO_GPT(context)->ocr_ch = ocr_ch;
  gptman->clk_src = gpt_clk;
  gptman->time_type = time_type;
  gptman->time_container = time_container;
  gptman->targetval = compval;
  gptman->callback = callback;
}

void
init_pitman(timer_manager_t * pitman,
            pit_speed_e       speedfield,
            pit_ch_e          pit_ch,
            clk_src_e         pit_clk,
            timetype_e        time_type,
            timer_s           time_container,
            uint32_t          ldval,
            timer_manager_cb  callback)
{
  void * context = pitman->timer_ctx->context;
  CONTEXT_TO_PIT(context)->pit_ch = pit_ch;
  CONTEXT_TO_PIT(context)->speedfield = speedfield;
  pitman->clk_src = pit_clk;
  pitman->time_type = time_type;
  pitman->time_container = time_container;
  pitman->targetval = ldval;
  pitman->callback = callback;
}

/**
 *
 * @brief Trigger a clock source reselect.
 *
 * @param timer A gpt_manager pointer which is basically a container with a
 *              callback attached to it
 * (NOTE: Selects based on the current value of timer->gpt_clk)
 **/
void
__slct_clksrc_gpt__(timer_manager_t * timer)
{
  void * context = timer->timer_ctx->context;
  switch (CONTEXT_TO_GPT(context)->gpt_x) {
    case GPT1_E:

      // Enable clock in CCM
      CCM_C_GPT1_EN;

      // Init clock registers
      GPT1_CR_EN(DISABLE);
      GPT1_IR_EN(DISABLE);
      GPT1_CR_SET_OM1(DISABLE);
      GPT1_CR_SET_OM2(DISABLE);
      GPT1_CR_SET_OM3(DISABLE);
      GPT1_CR_IM_CLR;
      GPT1_CR_CLKSRC(CONTEXT_TO_GPT(context)->gpt_clk);
      GPT1_CR_SWR(ENABLE);
      GPT1_SR_CLR;
      GPT1_CR_SET_ENMOD(ENABLE);
      GPT1_CR_EN(ENABLE);
      GPT1_IR_EN(ENABLE);
      break;
    case GPT2_E:
      CCM_C_GPT2_EN;
      GPT2_CR_EN(DISABLE);
      GPT2_IR_EN(DISABLE);
      GPT2_CR_SET_OM1(DISABLE);
      GPT2_CR_SET_OM2(DISABLE);
      GPT2_CR_SET_OM3(DISABLE);
      GPT2_CR_IM_CLR;
      GPT2_CR_CLKSRC(CONTEXT_TO_GPT(context)->gpt_clk);
      GPT2_CR_SWR(ENABLE);
      GPT2_SR_CLR;
      GPT2_CR_SET_ENMOD(ENABLE);
      GPT2_CR_EN(ENABLE);
      GPT2_IR_EN(ENABLE);
      break;
    default: break;
  }
}

/** p.2954 & p.2966
 *
 * 52.5.2.2 Output Compare
 *
 **/
void
__set_callback_gpt__(timer_manager_t * timer, timer_manager_cb callback)
{
  timer->callback = callback;
}

/**
 * @brief Clamps values if they exceed max limit of the 24mhz cycle registers
 * @details Clamps them using arithmetic logic, instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value,
 **/
uint32_t
__time_clamp_24MHz__(uint32_t intime, timetype_e ttype)
{
  uint32_t clamped_val = 0;
  clamped_val = 2U * (ttype == MINUTES_E && intime > 2U) +
                178U * (ttype == SECONDS_E && intime > 178U) +
                178956U * (ttype == MILLIS_E && intime > 178956U) +
                178956970U * (ttype == MICROS_E && intime > 178956970U) +
                1789569706U * (ttype == ZEPTOS_E && intime > 1789569706U) +
                0xffffffff * (ttype == YOCTOS_E && intime >= 0xffffffff) +
                0xffffffff * (ttype == NANOS_E && intime >= 0xffffffff);
  return (intime * (clamped_val == 0)) + clamped_val;
};

/**
 * @brief Clamps values if they exceed max limit of the count cycle registers
 *        50mhz cycles
 * @details Clamps them using arithmetic logic, instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value,
 **/
uint32_t
__time_clamp_50MHz__(uint32_t intime, timetype_e ttype)
{
  uint32_t clamped_val = 0;
  clamped_val = 1U * (ttype == MINUTES_E && intime > 1U) +
                85U * (ttype == SECONDS_E && intime > 85U) +
                85899U * (ttype == MILLIS_E && intime > 85899U) +
                85899345U * (ttype == MICROS_E && intime > 85899345U) +
                858993459U * (ttype == ZEPTOS_E && intime > 858993459U) +
                0xffffffff * (ttype == YOCTOS_E && intime >= 0xffffffff) +
                0xffffffff * (ttype == NANOS_E && intime >= 0xffffffff);
  return (intime * (clamped_val == 0)) + clamped_val;
};

/**
 * @brief Clamps values if they exceed max limit of the count cycle registers
 *        100mhz cycles
 * @details Clamps them using arithmetic logic, instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value,
 **/
uint32_t
__time_clamp_100MHz__(uint32_t intime, timetype_e ttype)
{
  if (ttype == MINUTES_E || ttype == HOURS_E || ttype == DAYS_E) {
    return 0x0;
  }
  uint32_t clamped_val = 0;
  clamped_val = 42U * (ttype == SECONDS_E && intime > 42U) +
                42949U * (ttype == MILLIS_E && intime > 42949U) +
                42949672U * (ttype == MICROS_E && intime > 42949672U) +
                429496729U * (ttype == ZEPTOS_E && intime > 429496729U) +
                0xffffffff * (ttype == YOCTOS_E && intime >= 0xffffffff) +
                0xffffffff * (ttype == NANOS_E && intime >= 0xffffffff);
  return (intime * (clamped_val == 0)) + clamped_val;
};

/**
 * @brief Clamps values if they exceed max limit of the count cycle registers
 *        150mhz cycles
 * @details Clamps them using arithmetic logic, instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value,
 **/
uint32_t
__time_clamp_150MHz__(uint32_t intime, timetype_e ttype)
{
  if (ttype == MINUTES_E || ttype == HOURS_E || ttype == DAYS_E) {
    return 0x0;
  }
  uint32_t clamped_val = 0;
  clamped_val = 28U * (ttype == SECONDS_E && intime > 28U) +
                28633U * (ttype == MILLIS_E && intime > 28633U) +
                28633115U * (ttype == MICROS_E && intime > 28633115U) +
                286331153U * (ttype == ZEPTOS_E && intime > 286331153U) +
                2863311530U * (ttype == YOCTOS_E && intime >= 2863311530U) +
                0xffffffff * (ttype == NANOS_E && intime >= 0xffffffff);
  return (intime * (clamped_val == 0)) + clamped_val;
};

/**
 * @brief Clamps values if they exceed max limit of the count cycle registers
 *        200mhz cycles
 * @details Clamps them using arithmetic logic, instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value,
 **/
uint32_t
__time_clamp_200MHz__(uint32_t intime, timetype_e ttype)
{
  if (ttype == MINUTES_E || ttype == HOURS_E || ttype == DAYS_E) {
    return 0x0;
  }
  uint32_t clamped_val = 0;
  clamped_val = 21U * (ttype == SECONDS_E && intime > 21U) +
                21474U * (ttype == MILLIS_E && intime > 21474U) +
                21474836U * (ttype == MICROS_E && intime > 21474836U) +
                214748364U * (ttype == ZEPTOS_E && intime > 214748364UL) +
                2147483647U * (ttype == YOCTOS_E && intime >= 2147483647U) +
                0xffffffff * (ttype == NANOS_E && intime >= 0xffffffff);
  return (intime * (clamped_val == 0)) + clamped_val;
};

// hardcoded to be used with the 24MHz clock for testing
uint32_t
__resolve_time_24MHz__(timer_manager_t * gptimer_mgr)
{
  timetype_e timetype = gptimer_mgr->time_type;
  // ensuring safety
  uint32_t time_og_unit =
      __time_clamp_24MHz__(gptimer_mgr->targetval, gptimer_mgr->time_type);
  if (time_og_unit == 0) {
    return 0x0;
  }

  // 24MHz = 41nanoseconds periods
  // (compareval * 0x16E3600 == compareval * 24MHz) (seconds)
  // (compareval * 0x00005dc0 == compareval * 24KHz) (millis)
  return (time_og_unit * 1440000000U) * (timetype == MINUTES_E) +
         (time_og_unit * 24000000U) * (timetype == SECONDS_E) +
         (time_og_unit * 24000U) * (timetype == MILLIS_E) +
         (time_og_unit * 24U) * (timetype == MICROS_E) +
         ((time_og_unit * 24U) / 10U) * (timetype == ZEPTOS_E) +
         ((time_og_unit * 24U) / 100U) * (timetype == YOCTOS_E) +
         ((time_og_unit * 24U) / 1000U) * (timetype == NANOS_E);
}

// hardcoded to be used with the 24MHz clock for testing
uint32_t
__resolve_time_50MHz__(timer_manager_t * timer_mgr)
{
  timetype_e timetype = timer_mgr->time_type;
  // ensuring safety
  uint32_t time_og_unit =
      __time_clamp_50MHz__(timer_mgr->targetval, timer_mgr->time_type);
  if (time_og_unit == 0) {
    return 0x0;
  }
  // 50MHz = 20nanoseconds periods
  // (compareval * 0x16E3600 == compareval * 24MHz) (seconds)
  // (compareval * 0x00005dc0 == compareval * 24KHz) (millis)
  return (time_og_unit * 3000000000U) * (timetype == MINUTES_E) +
         (time_og_unit * 50000000U) * (timetype == SECONDS_E) +
         (time_og_unit * 50000U) * (timetype == MILLIS_E) +
         (time_og_unit * 50U) * (timetype == MICROS_E) +
         ((time_og_unit * 50U) / 10U) * (timetype == ZEPTOS_E) +
         ((time_og_unit * 50U) / 100U) * (timetype == YOCTOS_E) +
         ((time_og_unit * 50U) / 1000U) * (timetype == NANOS_E);
}

// hardcoded to be used with the 24MHz clock for testing
uint32_t
__resolve_time_100MHz__(timer_manager_t * timer_mgr)
{
  timetype_e timetype = timer_mgr->time_type;
  // ensuring safety
  uint32_t time_og_unit =
      __time_clamp_100MHz__(timer_mgr->targetval, timer_mgr->time_type);
  if (time_og_unit == 0) {
    return 0x0;
  }

  // 100MHz = 10 nanoseconds periods
  // (compareval * 0x16E3600 == compareval * 24MHz) (seconds)
  // (compareval * 0x00005dc0 == compareval * 24KHz) (millis)
  return (time_og_unit * 100000000U) * (timetype == SECONDS_E) +
         (time_og_unit * 100000U) * (timetype == MILLIS_E) +
         (time_og_unit * 100U) * (timetype == MICROS_E) +
         (time_og_unit * 10U) * (timetype == ZEPTOS_E) +
         time_og_unit * (timetype == YOCTOS_E) +
         ((time_og_unit * 100U) / 1000U) * (timetype == NANOS_E);
}

// hardcoded to be used with the 24MHz clock for testing
uint32_t
__resolve_time_150MHz__(timer_manager_t * timer_mgr)
{
  timetype_e timetype = timer_mgr->time_type;
  // ensuring safety
  uint32_t time_og_unit =
      __time_clamp_150MHz__(timer_mgr->targetval, timer_mgr->time_type);
  if (time_og_unit == 0) {
    return 0x0;
  }

  // 150Hz = 6.67 nanoseconds periods
  // (compareval * 0x16E3600 == compareval * 24MHz) (seconds)
  // (compareval * 0x00005dc0 == compareval * 24KHz) (millis)
  return (time_og_unit * 150000000U) * (timetype == SECONDS_E) +
         (time_og_unit * 150000U) * (timetype == MILLIS_E) +
         (time_og_unit * 150U) * (timetype == MICROS_E) +
         (time_og_unit * 15U) * (timetype == ZEPTOS_E) +
         ((time_og_unit * 15U) / 10U) * (timetype == YOCTOS_E) +
         ((time_og_unit * 15U) / 100U) * (timetype == NANOS_E);
}

// hardcoded to be used with the 24MHz clock for testing
uint32_t
__resolve_time_200MHz__(timer_manager_t * timer_mgr)
{
  timetype_e timetype = timer_mgr->time_type;
  // ensuring safety
  uint32_t time_og_unit =
      __time_clamp_200MHz__(timer_mgr->targetval, timer_mgr->time_type);
  if (time_og_unit == 0) {
    return 0x0;
  }

  // 200MHz = 5 nanoseconds periods
  // (compareval * 0x16E3600 == compareval * 24MHz) (seconds)
  // (compareval * 0x00005dc0 == compareval * 24KHz) (millis)
  return (time_og_unit * 200000000U) * (timetype == SECONDS_E) +
         (time_og_unit * 200000U) * (timetype == MILLIS_E) +
         (time_og_unit * 200U) * (timetype == MICROS_E) +
         (time_og_unit * 20U) * (timetype == ZEPTOS_E) +
         (time_og_unit * 2U) * (timetype == YOCTOS_E) +
         ((time_og_unit * 2U) / 10U) * (timetype == NANOS_E);
}

void
__set_comparator_gpt__(timer_manager_t * timer)
{
  void * context = timer->timer_ctx->context;
  switch (CONTEXT_TO_GPT(context)->gpt_x) {
    case GPT1_E:
      // set GPT1f_IR fields: {ROVIE,IF1IE,IF2IE,OF1IE,OF2IE,OF3IE} = 1
      GPT1_IR_EN(ENABLE);

      /**
       * @todo @at Point to internal callback in irq vector table
       **/
      // add_to_irq_v(IRQ_GPT2, callback_gpt2);

      switch (CONTEXT_TO_GPT(context)->ocr_ch) {
        case OCR_CH1:
          GPT1_CR_SET_OM1(0x3);
          //*glob_gpt_ptrs[0] = __resolve_time__(timer); // timer->compval;
          GPT1_OCR1 = __resolve_time_24MHz__(timer); // timer->compval;
          break;
        case OCR_CH2:
          GPT1_CR_SET_OM2(0x3);
          // *glob_gpt_ptrs[1] = __resolve_time__(timer); // timer->compval;
          GPT1_OCR2 = __resolve_time_24MHz__(timer); // timer->compval;
          break;
        case OCR_CH3:
          GPT1_CR_SET_OM3(0x3);
          // *glob_gpt_ptrs[2] = __resolve_time__(timer); // timer->compval;
          GPT1_OCR3 = __resolve_time_24MHz__(timer); // timer->compval;
          break;
        default: break;
      }
      break;

    case GPT2_E:
      // set GPT2_IR fields: {ROVIE,IF1IE,IF2IE,OF1IE,OF2IE,OF3IE} = 1
      GPT2_IR_EN(ENABLE);

      /**
       * @todo @at Point to internal callback in irq vector table
       **/
      // add_to_irq_v(IRQ_GPT2, callback_gpt2);

      switch (CONTEXT_TO_GPT(context)->ocr_ch) {
        case OCR_CH1:
          GPT2_CR_SET_OM1(0x3);
          // *glob_gpt_ptrs[3] = __resolve_time__(timer); // timer->compval;
          GPT2_OCR1 = __resolve_time_24MHz__(timer); // timer->compval;
          break;
        case OCR_CH2:
          GPT2_CR_SET_OM2(0x3);
          // *glob_gpt_ptrs[4] = __resolve_time__(timer); // timer->compval;
          GPT2_OCR2 = __resolve_time_24MHz__(timer); // timer->compval;
          break;
        case OCR_CH3:
          GPT2_CR_SET_OM3(0x3);
          // *glob_gpt_ptrs[5] = __resolve_time__(timer); // timer->compval;
          GPT2_OCR3 = __resolve_time_24MHz__(timer); // timer->compval;
          break;
        default: break;
      }
  }
}

/**
 * @brief Set time value for comparator
 *
 * @param timer input, Ptr to a gpt_manager
 * @param time_type input, enum of gp_timetype_e
 * @param compareval input, value, in the timetype requested
 *
 * @todo Expand it to work with more than just 24MHz clocks later, right now
 * assume 24MHz
 *
 * @note Count registers are too small to keep track of days on the 24MHz clock,
 * max would be about 3.2 hours, will have to create an hour-incrementor
 * which could either be 8 or 16 bits wide, 8 bits seems fine to focus on at
 * first, it would give about 10 days of span, and would only be used if either
 * hour or day time_types are used.
 */
void
set_time(timer_manager_t * timer, timetype_e time_type, gpt_ocr_t compareval)
{
  timer->targetval = compareval;
  timer->time_type = time_type;
  __slct_clksrc_gpt__(timer);
  __set_comparator_gpt__(timer);
  __setup_gpt2__(); // test for now, will make a general version for both gpt1
                    // and 2 maybe
}

void
__setup_gpt2__()
{
  // pinMode(13, OUTPUT);
  // analogWriteFrequency(14, 100);  // test with PWM
  // analogWrite(14, 128); // jumper pwm 14  to pin 15  Serial3 on T4B2 breakout
  // Connect GPS 1PPS signal to pin 15 (GPIO_AD_B1_03)
  IOMUXC_GPT2_IPP_IND_CAPIN1__SLCT_IN_DR = 0x1; // remap GPT2 capture 1
  IOMUXC_MUX_PAD_GPIO_AD_B1_CR03 = 0x8; // GPT2 Capture1
  IOMUXC_PAD_PAD_GPIO_AD_B1_CR03 = PAD_HYST_ENABLED | PAD_100KOHM_DOWN |
                                   PAD_SLCT_PULLER | PAD_PULLKEEP_ENABLED;

  GPT2_CR = DISABLE;
  GPT2_PR = DISABLE;
  CCM_C_GPT2_EN;

  GPT2_SR_CLR; // clear all prior status
  GPT2_IR_EN(ENABLE);
  GPT2_CR = GPT_CR_SET_EN(GPT2_CR, ENABLE) |
            GPT_CR_CLKSRC(GPT2_CR, SRC_IPG_CLK) | GPT_CR_MODE(RESTARTMODE_E) |
            GPT_CR_IM1_SET(GPT2_CR, ENABLE);

  // NVIC_ENABLE_IRQ(IRQ_GPT2);
}

void
setup_pit_timer(timer_manager_t * pit_mgr, pit_timer_e pit_timerx)
{
  add_to_irq_v(IRQ_PIT, pit_mgr->callback);
  reset_pit(pit_mgr, pit_timerx);
}

void
reset_pit(timer_manager_t * pit_mgr, pit_timer_e pit_timerx)
{
  // turn on PIT
  PIT_MCR_SET(MCR_RESET);
  // Push callback to interrupt vector
  pit_context_t * pitctx = CONTEXT_TO_PIT(pit_mgr->timer_ctx->context);
  if (pitctx->speedfield == PIT_SPEED_50MHz) {
    PIT_LOADVAL0_SET(__resolve_time_50MHz__(pit_mgr));
  }

  if (pitctx->speedfield == PIT_SPEED_100MHz) {
    PIT_LOADVAL0_SET(__resolve_time_100MHz__(pit_mgr));
  }

  if (pitctx->speedfield == PIT_SPEED_150MHz) {
    PIT_LOADVAL0_SET(__resolve_time_150MHz__(pit_mgr));
  }

  if (pitctx->speedfield == PIT_SPEED_200MHz) {
    PIT_LOADVAL0_SET(__resolve_time_200MHz__(pit_mgr));
  }

  if (pit_timerx == PIT_TIMER_00) {
    PIT_TCTRL0_IRQ_EN(ENABLE); // enable  interrupts
    PIT_TCTRL0_TIMER_EN(ENABLE); // start Timer 3
  }
  if (pit_timerx == PIT_TIMER_01) {
    PIT_TCTRL1_IRQ_EN(ENABLE); // enable Timer 3 interrupts
    PIT_TCTRL1_TIMER_EN(ENABLE); // start Timer 3
  }
  if (pit_timerx == PIT_TIMER_02) {
    PIT_TCTRL2_IRQ_EN(ENABLE); // enable Timer 3 interrupts
    PIT_TCTRL2_TIMER_EN(ENABLE); // start Timer 3
  }
  if (pit_timerx == PIT_TIMER_03) {
    PIT_TCTRL3_IRQ_EN(ENABLE); // enable Timer 3 interrupts
    PIT_TCTRL3_TIMER_EN(ENABLE); // start Timer 3
  }
}
