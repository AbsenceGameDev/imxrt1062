#include "gp_timer.h"

// Globals
gpt_manager glob_gptman[6];
vuint32_t * glob_gpt_ptrs[6] = {
    &GPT1_OCR1, &GPT1_OCR2, &GPT1_OCR3, &GPT2_OCR1, &GPT2_OCR2, &GPT2_OCR3};

void
init_gptman()
{
  for (unsigned char idx = 0; idx < 6; idx++) {
    glob_gptman[idx].time_container.steps = 0x0;
    glob_gptman[idx].time_container.count.s = 0x0;
    glob_gptman[idx].time_type = SECONDS_E;
    glob_gptman[idx].callback = ((timer_manager_cb)0);
    glob_gptman[idx].gpt_x = (GPT1_E * (idx < 3)) + (GPT2_E * !(idx < 3));
    glob_gptman[idx].compval = 0x0;
    glob_gptman[idx].gpt_clk = GPT_NO_CLK;
  }
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
      GPT1_CR_EN(0x0);
      GPT1_IR_EN(0x0);
      GPT1_CR_SET_OM1(0x0);
      GPT1_CR_SET_OM2(0x0);
      GPT1_CR_SET_OM3(0x0);
      GPT1_CR_IM_CLR;
      GPT1_CR_CLKSRC(timer->gpt_clk);
      GPT1_CR_SWR(0x1);
      GPT1_SR_CLR;
      GPT1_CR_SET_ENMOD(0x1);
      GPT1_CR_EN(0x1);
      GPT1_IR_EN(0x1);
      break;
    case GPT2_E:
      CCM_C_GPT2_EN;
      GPT2_CR_EN(0x0);
      GPT2_IR_EN(0x0);
      GPT2_CR_SET_OM1(0x0);
      GPT2_CR_SET_OM2(0x0);
      GPT2_CR_SET_OM3(0x0);
      GPT2_CR_IM_CLR;
      GPT2_CR_CLKSRC(timer->gpt_clk);
      GPT2_CR_SWR(0x1);
      GPT2_SR_CLR;
      GPT2_CR_SET_ENMOD(0x1);
      GPT2_CR_EN(0x1);
      GPT2_IR_EN(0x1);
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

void
__set_comparator_gpt__(gpt_manager * timer,
                       gpt_ocr_t     compareval,
                       gpt_ocr_e     channel)
{
  switch (timer->gpt_x) {
    case GPT1_E:
      // set GPT1f_IR fields: {ROVIE,IF1IE,IF2IE,OF1IE,OF2IE,OF3IE} = 1
      GPT1_IR_EN(0x1);

      // Point to internal callback in irq vector table
      add_to_irq_vector(IRQ_GPT1, callback_gpt1);

      switch (channel) {
        case OCR_CH1: *glob_gpt_ptrs[0] = compareval; break;
        case OCR_CH2: *glob_gpt_ptrs[1] = compareval; break;
        case OCR_CH3: *glob_gpt_ptrs[2] = compareval; break;
        default: break;
      }
      break;

    case GPT2_E:
      // set GPT2_IR fields: {ROVIE,IF1IE,IF2IE,OF1IE,OF2IE,OF3IE} = 1
      GPT2_IR_EN(0x1);

      // Point to internal callback in irq vector table
      add_to_irq_vector(IRQ_GPT2, callback_gpt2);

      switch (channel) {
        case OCR_CH1: *glob_gpt_ptrs[3] = compareval; break;
        case OCR_CH2: *glob_gpt_ptrs[4] = compareval; break;
        case OCR_CH3: *glob_gpt_ptrs[5] = compareval; break;
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

  // 24MHz = 41nanoseconds periods
  // (compareval * 0x16E3600) (seconds)
  uint32_t     ref_value_24M = compareval * 0x16E3600;
  gp_timer_u * u_time_ptr = &(timer->time_container.count);

  switch (time_type) {
    case DAYS_E:
      __set_comparator_gpt__(
          timer, u_time_ptr->ns = (ref_value_24M * 85200), timer->ocr_ch);
      break; // 0.00000000000048229167d resolution
    case HOURS_E:
      __set_comparator_gpt__(
          timer, u_time_ptr->ns = (ref_value_24M * 3600), timer->ocr_ch);
      break; // 0.000000000011575h resolution
    case MINUTES_E:
      __set_comparator_gpt__(
          timer, u_time_ptr->ns = (ref_value_24M * 60), timer->ocr_ch);
      break; // 0.0000000006945m resolution
    case SECONDS_E:
      __set_comparator_gpt__(
          timer, u_time_ptr->ns = ref_value_24M, timer->ocr_ch);
      break; // 0.00000004167s reolution.
    case MILLIS_E:
      __set_comparator_gpt__(
          timer, u_time_ptr->ns = (ref_value_24M / 1000), timer->ocr_ch);
      break; // 0.00004167ms resolution
    case MICROS_E:
      __set_comparator_gpt__(
          timer, u_time_ptr->ns = (ref_value_24M / 1000000), timer->ocr_ch);
      break; // 0.04167us resolution
    case ZEPTOS_E:
      __set_comparator_gpt__(
          timer, u_time_ptr->ns = (ref_value_24M / 10000000), timer->ocr_ch);
      break; // 0.4167zs resolution
    case YOCTOS_E:
      __set_comparator_gpt__(
          timer, u_time_ptr->ns = (ref_value_24M / 100000000), timer->ocr_ch);
      break; // 4.167ys resolution
    case NANOS_E:
      __set_comparator_gpt__(
          timer, u_time_ptr->ns = (ref_value_24M / 1000000000), timer->ocr_ch);
      break; // 41.67ns resolution

    default: break;
  }
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
    }
  }
}

void
callback_gpt2(void)
{
  for (uint8_t idx = 3; idx < 6; idx++) {
    if (glob_gptman[idx].compval == *glob_gpt_ptrs[idx]) {
      glob_gptman[idx].callback(); // Trigger manager interface callback
    }
  }
}