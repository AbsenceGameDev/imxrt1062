#include "gp_timer.h"

/**
 * @todo Map interrupt registers?
 *       Set up trigger on count?
 *         - Can l use interrupt registers to be trigger on count values?
 *            - If NOT? Then investigate what might be used for this,
 *              and read the specification again, look close at the
 *              timers and clock chapters but also chapter 4 on interrupts
 **/
const void
__slct_clksrc_gpt__(gpt_manager * timer)
{
  // Set up PMU registers? (Power Management Unit, chapter 16)

  // Map interrupt registers?

  /**
   * To trigger some form of callback I seem to be able to use the timer
   * comparators, It seems to be hardware module(s) in armv7 which compares a
   * given value to the current counter calue, Specs say it does not interfere
   * or lock hte register.it can read it without any performance cost at all.
   **/

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
const void
__set_comparator_gpt__(gpt_manager * timer,
                       gpt_ocr_t     compareval,
                       gpt_ocr_e     channel)
{
  /** @todo set up interrupts somewhere in here, read above excerpt from spec! */

  switch (timer->gpt_x) {
    case GPT1_E:
      // set GPT1f_IR fields: {ROVIE,IF1IE,IF2IE,OF1IE,OF2IE,OF3IE} = 1
      GPT1_IR_EN(0x1);

      switch (channel) {
        case OCR_CH1: GPT1_OCR1 = compareval; break;
        case OCR_CH2: GPT1_OCR2 = compareval; break;
        case OCR_CH3: GPT1_OCR3 = compareval; break;
        default: break;
      }
      break;

    case GPT2_E:
      // set GPT2_IR fields: {ROVIE,IF1IE,IF2IE,OF1IE,OF2IE,OF3IE} = 1
      GPT2_IR_EN(0x1);

      switch (channel) {
        case OCR_CH1: GPT2_OCR1 = compareval; break;
        case OCR_CH2: GPT2_OCR2 = compareval; break;
        case OCR_CH3: GPT2_OCR3 = compareval; break;
        default: break;
      }
      break;
  }
}