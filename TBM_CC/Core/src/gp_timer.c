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
__set_comparator_gpt__(gpt_manager *    timer,
                       gpt_ocr_t        compareval,
                       gpt_ocr_e        channel,
                       timer_manager_cb callback)
{
  /** @todo set up interrupts somewhere in here, read above excerpt from spec! */
  timer->callback = callback; // Supplied interface callback

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