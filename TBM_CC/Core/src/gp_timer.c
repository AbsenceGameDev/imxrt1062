#include "gp_timer.h"

// Globals
gpt_manager glob_gptman[6];
vuint32_t * glob_gpt_ptrs[6] = {
    &GPT1_OCR1, &GPT1_OCR2, &GPT1_OCR3, &GPT2_OCR1, &GPT2_OCR2, &GPT2_OCR3};

void
init_gptman_arr()
{
  for (unsigned char idx = 0; idx < 6; idx++) {
    glob_gptman[idx].time_container.steps = 0x0;
    glob_gptman[idx].time_container.count.s = 0x0;
    glob_gptman[idx].time_type = SECONDS_E;
    glob_gptman[idx].callback =
        ((timer_manager_cb)0); // firs setting a null callback
    glob_gptman[idx].gpt_x = (GPT1_E * (idx < 3)) + (GPT2_E * !(idx < 3));
    glob_gptman[idx].compval = 0x0;
    glob_gptman[idx].gpt_clk = GPT_NO_CLK;
  }
}

void
init_gptman(gpt_manager *    gptman,
            gptimer_e        gpt_x,
            gpt_ocr_e        ocr_ch,
            clk_src_e        gpt_clk,
            gp_timetype_e    time_type,
            gp_timer_s       time_container,
            uint32_t         compval,
            timer_manager_cb callback)
{
  gptman->gpt_x = gpt_x;
  gptman->ocr_ch = ocr_ch;
  gptman->gpt_clk = gpt_clk;
  gptman->time_type = time_type;
  gptman->time_container = time_container;
  gptman->compval = compval;
  gptman->callback = callback;
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
__slct_clksrc_gpt__(gpt_manager * timer)
{
  switch (timer->gpt_x) {
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
      GPT1_CR_CLKSRC(timer->gpt_clk);
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
      GPT2_CR_CLKSRC(timer->gpt_clk);
      GPT2_CR_SWR(ENABLE);
      GPT2_SR_CLR;
      GPT2_CR_SET_ENMOD(ENABLE);
      GPT2_CR_EN(ENABLE);
      GPT2_IR_EN(ENABLE);
      break;
  }
}

/** p.2954 & p.2966
 *
 * 52.5.2.2 Output Compare
 * The three Output Compare Channels use the same counter (GPT_CNT) as the
 * Input Capture Channels.
 * When the programmed content of an Output Compare register matches the
 * value in GPT_CNT, an output compare status flag is set and an interrupt
 * is generated (if the corresponding bit is set in the interrupt register).
 *
 * Consequently, the Output Compare timer pin will be set, cleared, toggled,
 * not affected at all or provide anactive-low pulse for one input clock period
 * (subject to the restriction on the maximumfrequency allowed on the pad)
 * according to the mode bits (that were programmed).
 *
 *
 * There is also a "forced-compare" feature mentioned in the specs, which may be
 *  irrelevant right now but it's definelty worth coming back to later.
 *
 **/
void
__set_callback_gpt__(gpt_manager * timer, timer_manager_cb callback)
{
  timer->callback = callback;
}

/**
 * @brief Clamps values if they exceed max limit of the 24mhx cycle registers
 * @details Clamps them using arithmetic logic, instead of control-blocks like
 *ifs
 *
 * @returns Either return a clamped value, or return the inputted value,
 * if the time didn't exceed the max of it's timetype, then return intputted
 *value, otherwise return clamped value
 **/
uint32_t
__time_clamp_24mhz__(uint32_t intime, gp_timetype_e ttype)
{
  uint32_t clamped_val = 0;
  clamped_val = 2 * (ttype == MINUTES_E && intime > 2) +
                178 * (ttype == SECONDS_E && intime > 178) +
                178956 * (ttype == MILLIS_E && intime > 178956) +
                178956970 * (ttype == MICROS_E && intime > 178956970) +
                1789569706 * (ttype == ZEPTOS_E && intime > 1789569706) +
                0xffffffff * (ttype == YOCTOS_E && intime >= 0xffffffff) +
                0xffffffff * (ttype == NANOS_E && intime >= 0xffffffff);
  return (intime * (clamped_val == 0)) + clamped_val;
};

// hardcoded to be used with the 24MHz clock for testing
uint32_t
__resolve_time__(gpt_manager * gptimer_mgr)
{
  gp_timetype_e timetype = gptimer_mgr->time_type;
  // ensuring safety
  uint32_t time_og_unit =
      __time_clamp_24mhz__(gptimer_mgr->compval, gptimer_mgr->time_type);

  // 24MHz = 41nanoseconds periods
  // (compareval * 0x16E3600 == compareval * 24MHz) (seconds)
  // (compareval * 0x00005dc0 == compareval * 24KHz) (millis)
  return (time_og_unit * 0x55d4a800) * (timetype == MINUTES_E) +
         (time_og_unit * 0x016e3600) * (timetype == SECONDS_E) +
         (time_og_unit * 0x00005dc0) * (timetype == MILLIS_E) +
         (time_og_unit * 0x00000018) * (timetype == MICROS_E) +
         ((time_og_unit * 0x18) / 0xa) * (timetype == ZEPTOS_E) +
         ((time_og_unit * 0x18) / 0x64) * (timetype == YOCTOS_E) +
         ((time_og_unit * 0x18) / 0x3e8) * (timetype == NANOS_E);
}

void
__set_comparator_gpt__(gpt_manager * timer)
{
  switch (timer->gpt_x) {
    case GPT1_E:
      // set GPT1f_IR fields: {ROVIE,IF1IE,IF2IE,OF1IE,OF2IE,OF3IE} = 1
      GPT1_IR_EN(ENABLE);

      // Point to internal callback in irq vector table
      add_to_irq_v(IRQ_GPT1, timer->callback);

      switch (timer->ocr_ch) {
        case OCR_CH1:
          GPT1_CR_SET_OM1(0x3);
          //*glob_gpt_ptrs[0] = __resolve_time__(timer); // timer->compval;
          GPT1_OCR1 = __resolve_time__(timer); // timer->compval;
          break;
        case OCR_CH2:
          GPT1_CR_SET_OM2(0x3);
          // *glob_gpt_ptrs[1] = __resolve_time__(timer); // timer->compval;
          GPT1_OCR2 = __resolve_time__(timer); // timer->compval;
          break;
        case OCR_CH3:
          GPT1_CR_SET_OM3(0x3);
          // *glob_gpt_ptrs[2] = __resolve_time__(timer); // timer->compval;
          GPT1_OCR3 = __resolve_time__(timer); // timer->compval;
          break;
        default: break;
      }
      break;

    case GPT2_E:
      // set GPT2_IR fields: {ROVIE,IF1IE,IF2IE,OF1IE,OF2IE,OF3IE} = 1
      GPT2_IR_EN(ENABLE);

      // Point to internal callback in irq vector table
      add_to_irq_v(IRQ_GPT2, callback_gpt2);

      switch (timer->ocr_ch) {
        case OCR_CH1:
          GPT2_CR_SET_OM1(0x3);
          // *glob_gpt_ptrs[3] = __resolve_time__(timer); // timer->compval;
          GPT2_OCR1 = __resolve_time__(timer); // timer->compval;
          break;
        case OCR_CH2:
          GPT2_CR_SET_OM2(0x3);
          // *glob_gpt_ptrs[4] = __resolve_time__(timer); // timer->compval;
          GPT2_OCR2 = __resolve_time__(timer); // timer->compval;
          break;
        case OCR_CH3:
          GPT2_CR_SET_OM3(0x3);
          // *glob_gpt_ptrs[5] = __resolve_time__(timer); // timer->compval;
          GPT2_OCR3 = __resolve_time__(timer); // timer->compval;
          break;
        default: break;
      }
      break;
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
set_time(gpt_manager * timer, gp_timetype_e time_type, gpt_ocr_t compareval)
{
  timer->compval = compareval;
  timer->time_type = time_type;
  __slct_clksrc_gpt__(timer);
  __set_comparator_gpt__(timer);
  __setup_gpt2__(); // test for now, will make a general version for both gpt1
                    // and 2 maybe
}

/**
 * GPT External Signals table
 *| NAME            | PAD          | MODE |
 *|=================|==============|======|
 *| GPT1_COMPARE1   | GPIO_EMC_35  | ALT2 |
 *|                 | GPIO_B1_07   | ALT8 |
 *|=================|==============|======|
 *| GPT1_COMPARE2   | GPIO_EMC_35  | ALT2 |
 *|                 | GPIO_B1_07   | ALT8 |
 *|=================|==============|======|
 *| GPT1_COMPARE3   | GPIO_EMC_35  | ALT2 |
 *|                 | GPIO_B1_07   | ALT8 |
 *|=================|==============|======|
 *| GPT2_COMPARE1   | GPIO_EMC_35  | ALT2 |
 *|                 | GPIO_B1_07   | ALT8 |
 *|=================|==============|======|
 *| GPT2_COMPARE2   | GPIO_EMC_35  | ALT2 |
 *|                 | GPIO_B1_07   | ALT8 |
 *|=================|==============|======|
 *| GPT2_COMPARE3   | GPIO_EMC_35  | ALT2 |
 *|                 | GPIO_B1_07   | ALT8 |
 *|=================|==============|======|
 */
void
callback_gpt1(void)
{
  for (uint8_t idx = 0; idx < 3; idx++) {
    if (glob_gptman[idx].compval == *glob_gpt_ptrs[idx]) {
      glob_gptman[idx].callback(); // Trigger manager interface callback
      set_time(&glob_gptman[idx],
               glob_gptman[idx].time_type,
               ++(glob_gptman[idx].compval));
    }
  }
}

void
callback_gpt2(void)
{
  for (uint8_t idx = 3; idx < 6; idx++) {
    if (glob_gptman[idx].compval == *glob_gpt_ptrs[idx]) {
      glob_gptman[idx].callback(); // Trigger manager interface callback
      set_time(&glob_gptman[idx],
               glob_gptman[idx].time_type,
               ++(glob_gptman[idx].compval));
    }
  }
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
  GPT2_CR = GPT_CR_SET_EN(ENABLE) | GPT_CR_CLKSRC(GPT_IPG_CLK) |
            GPT_CR_MODE(RESTARTMODE_E) | GPT_CR_IM1_SET(ENABLE);

  // NVIC_ENABLE_IRQ(IRQ_GPT2);
}
