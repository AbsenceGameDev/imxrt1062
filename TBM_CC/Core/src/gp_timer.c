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
slct_clksrc_gpt(gp_timer_s * timer)
{
  switch (timer->gpt_x) {
    case GPT1_E:
      // Map interrupt registers?

      // Set up trigger on count? Canl

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