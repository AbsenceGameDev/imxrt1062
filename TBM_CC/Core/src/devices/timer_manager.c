/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2026, MIT-License included in project toplevel dir
 */

#include "timer_manager.h"

#include "gpio_handler.h"

#include "sys/registers.h"

#include "iomux_controller.h"

// Globals
timer_manager_s glob_gptman[6];
vuint32_t*     glob_gpt_ptrs[6] = {&GPT1_OCR1, &GPT1_OCR2, &GPT1_OCR3, &GPT2_OCR1, &GPT2_OCR2, &GPT2_OCR3};

/* General Purpose Timers (GPT) */

void
init_gptman(timer_manager_s* restrict gptman,
            gptx_e           gpt_x,
            gpt_ocr_e        ocr_ch,
            clk_src_e        gpt_clk,
            timetype_e       time_type,
            uint32_t         compval,
            timer_manager_cb interrupt_callback)
{
  void* context = gptman->timer_ctx->context;
  CONTEXT_TO_GPT(context)->gpt_x = gpt_x;
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
__slct_clksrc_gpt__(timer_manager_s* restrict gptman)
{
  void* context = gptman->timer_ctx->context;
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
__set_callback_gpt__(timer_manager_s* restrict gptman,
                     timer_manager_cb interrupt_callback)
{
  gptman->interrupt_callback = interrupt_callback;
}

void
__set_comparator_gpt__(timer_manager_s* restrict gptman)
{
  void * context = gptman->timer_ctx->context;
  switch (CONTEXT_TO_GPT(context)->gpt_x) {
    case GPT1_E:
      // set GPT1f_IR fields: {ROVIE,IF1IE,IF2IE,OF1IE,OF2IE,OF3IE} = 1
      GPT1_IR_EN(ENABLE);

      /**
       * @todo @at Point to internal callback in irq vector table
       **/
      // add_to_irq_v(IRQ_GPT2, callback_gpt2);

      switch (CONTEXT_TO_GPT(context)->ocr_ch) 
      {
        case OCR_CH1:
          GPT1_CR_SET_OM1(0x3);
          GPT1_OCR1 =
              __resolve_time_24MHz__(gptman->targetval, gptman->time_type, FROMTIME_E);
          break;
        case OCR_CH2:
          GPT1_CR_SET_OM2(0x3);
          GPT1_OCR2 =
              __resolve_time_24MHz__(gptman->targetval, gptman->time_type, FROMTIME_E);
          break;
        case OCR_CH3:
          GPT1_CR_SET_OM3(0x3);
          GPT1_OCR3 =
              __resolve_time_24MHz__(gptman->targetval, gptman->time_type, FROMTIME_E);
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

      switch (CONTEXT_TO_GPT(context)->ocr_ch) 
      {
        case OCR_CH1:
          GPT2_CR_SET_OM1(0x3);
          GPT2_OCR1 =
              __resolve_time_24MHz__(gptman->targetval, gptman->time_type, FROMTIME_E);
          break;
        case OCR_CH2:
          GPT2_CR_SET_OM2(0x3);
          GPT2_OCR2 =
              __resolve_time_24MHz__(gptman->targetval, gptman->time_type, FROMTIME_E);
          break;
        case OCR_CH3:
          GPT2_CR_SET_OM3(0x3);
          GPT2_OCR3 =
              __resolve_time_24MHz__(gptman->targetval, gptman->time_type, FROMTIME_E);
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
 * @param compval input, value, in the timetype requested
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
set_time(timer_manager_s* restrict gptman,
         timetype_e time_type,
         gpt_ocr_t  compval)
{
  gptman->targetval = compval;
  gptman->time_type = time_type;
  __slct_clksrc_gpt__(gptman);
  __set_comparator_gpt__(gptman);
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
  IOMUXC_MUX_PAD_GPIO_AD_B1_CR03 = 0x8;         // GPT2 Capture1
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

/* Periodic Interrupt Timers (PIT) */

void
init_pitman(timer_manager_s* restrict pitman,
            timer_datum_s* restrict   timerdatum,
            pit_ch_e              pit_ch,
            timer_manager_cb      interrupt_callback,
            timer_manager_sick_cb tick_callback)
{
  clk_src_e pit_clk = SRC_NO_CLK;
  switch (timerdatum->freq)
  {
  case PIT_SPEED_50MHz:
    pit_clk = SRC_IPG_CLK;
    break;
  case PIT_SPEED_100MHz: case PIT_SPEED_150MHz: case PIT_SPEED_200MHz:
    pit_clk = SRC_IPG_CLK_HIFREQ;
    break;
  }

  void* context = pitman->timer_ctx->context;
  CONTEXT_TO_PIT(context)->pit_ch = pit_ch;
  CONTEXT_TO_PIT(context)->freq = timerdatum->freq;

  pitman->clk_src            = pit_clk;
  pitman->time_type          = timerdatum->type;
  pitman->interrupt_callback = interrupt_callback;
  pitman->keep_ticking = 1; // keep tick alive, and be used to kill the tick within itself if some condition is met

  pitman->tick_callback      = tick_callback;
  
  pitman->targetval = resolve_time(timerdatum->freq, timerdatum->type, timerdatum->val, FROMTIME_E);
}

//
// Work-around until I figure out my mistake in the resolve time functions
#define base_div(x) (24)*(x); targetval *= 10;
uint32_t resolve_time(pit_speed_e freq, timetype_e time_type, uint32_t targetval, ttconversiondir_e conversiondir)
{
  switch (freq) 
  {
    case PIT_SPEED_50MHz:
      targetval  = __resolve_time_50MHz__(targetval, time_type, conversiondir); 
      // targetval /= base_div(1);
      break;
    case PIT_SPEED_100MHz:
      targetval  = __resolve_time_100MHz__(targetval, time_type, conversiondir);
      // targetval /= base_div(2);
      break;
    case PIT_SPEED_150MHz:
      targetval  = __resolve_time_150MHz__(targetval, time_type, conversiondir);
      // targetval /= base_div(3);
      break;
    case PIT_SPEED_200MHz:
      targetval  = __resolve_time_200MHz__(targetval, time_type, conversiondir);
      // targetval /= base_div(4);
      break;

    default: break;
  }  

  return targetval;
}

float resolve_time_as_float(pit_speed_e freq, timetype_e time_resolution, uint32_t targetval, ttconversiondir_e conversiondir)
{
    float fDeltaTime = resolve_time(freq, time_resolution, targetval, conversiondir);
    punned_float_u punned_float = {.AsInt32 = fDeltaTime};

    float fDivisor = 1.0;
    switch (time_resolution)
    {
    case DAYS_E: fDivisor = 1.f / 86400.f; break;
    case HOURS_E: fDivisor = 1.f / 3600.f; break;
    case MINUTES_E: fDivisor = 1.f / 60.f; break;
    case MILLIS_E: fDivisor = 1000.f; break; // milli-seconds, 10E-3
    case MICROS_E: fDivisor = 1000000.f; break; // micro-seconds, 10E-6
    case ZETTOS_E: fDivisor = 10000000.f; break; // zetto-seconds, 10E-7
    case YOTTOS_E: fDivisor = 100000000.f; break; // yotto-seconds, 10E-8
    case NANOS_E: fDivisor = 1000000000.f; break;  // nano-seconds,  10E-9
    default: break;
    }

    return punned_float.AsFloat / fDivisor; // convert from milliseconds to seconds  
}


timer_datum_s generate_time_struct(pit_speed_e freq, timetype_e type, uint32_t timeval)
{
  timeval                  = resolve_time(freq, type, timeval, FROMTIME_E);
  timer_datum_s timerdatum = {.type = type, .freq = freq, .val = timeval};
  return timerdatum;
}

timer_manager_cb*       GCachedInterruptCallbackPIT;
void dummy_tick(float dummy){ return; };
void setup_PITx(timer_manager_s* restrict pitman)
{
  //
  // Resolve the previously stored timer context
  void* ctx_ = pitman->timer_ctx->context;
  pit_speed_e freq = CONTEXT_TO_PIT(ctx_)->freq;
  
  // Set the interrupt callback and enable the interrupt vector
  if (pitman->interrupt_callback != NULL) 
  { 
    if ((&pitman->interrupt_callback) != GCachedInterruptCallbackPIT) 
    {
      GCachedInterruptCallbackPIT = &pitman->interrupt_callback;
      add_to_irq_v(IRQ_PIT, pitman->interrupt_callback);
    }

    NVIC_SET_PRIORITY(IRQ_PIT, 0x0);
    NVIC_ENABLE_IRQ(IRQ_PIT);
  }

  // Select an appropriate clock root
  perclk_src_e perclk_root; 
  switch (freq)
  {
  case PIT_SPEED_50MHz:  case PIT_SPEED_100MHz: 
  case PIT_SPEED_150MHz: case PIT_SPEED_200MHz:
    perclk_root = IPG_ROOT;
    break;

  default:
    perclk_root = OSC_ROOT;
    break;
  }

  // Set the PIT clock source and clock root divider, then set flag to allow enabling the PIT
  CCM_SLCT_PERCLK_SRC(perclk_root);
  CCM_SCMUX1_DIV_SET(PC_DIV7);  // Divider for perclk podf 
  CCM_SET_PIT_ENABLE(CLK_ON_ALL_MODES);

  // According to the processor errata there is a bug that requires us to clear the PIT ctrl register first time using it
  if (PIT_MCR == 2) 
  {
    // first time, fix POR values
    PIT_TCTRL0 = 0;
    PIT_TCTRL1 = 0;
  }

  CCM_C_MEOR |= 0x1;
  // PMU_MISC2_SET = 0;

  // 1. Selects PIT Trigger enabled in the given MUX register (11.7.47 in the i.MXRT1060 manual), 
  // 2. Selects frequency in the given control register (11.7.171 in the i.MXRT1060 manual)
  IOMUXC_MUX_PAD_GPIO_AD_B0_CR04 &= (~0x3 | MM_ALT6); 
  IOMUXC_PAD_PAD_GPIO_AD_B0_CR04 &= (~(0x3 << 0x6) | (freq << 0x6)); 

  // Reset pit registers due to bootimage config, p200 ref man
  PIT_MCR = 1;
  PIT_LDVAL0 = PIT_LDVAL1 = PIT_LDVAL2 = PIT_LDVAL3 = 0;
  PIT_TCTRL0 = PIT_TCTRL1 = PIT_TCTRL2 = PIT_TCTRL3 = 0;
  PIT_TFLG0  = PIT_TFLG1  = PIT_TFLG2  = PIT_TFLG3  = 1;

  IOMUXC_GPR_GPR01 &= ~(0x1 << 0xc) | (0x1 << 0xc); // Set bit at idx 0xc (12)
  PIT_MCR_SET(MCR_RESET);

  // Set the PIT load value and enable the timer
  PIT_LDVAL0 = PIT_LDVAL1 = PIT_LDVAL2 = PIT_LDVAL3 = pitman->targetval;
  PIT_TCTRL0 = 3; PIT_TCTRL1 = PIT_TCTRL2 = PIT_TCTRL3 = 2; // Only enable Timer 1, figure out proper abstractions later
}

void
setup_pit_irq(timer_manager_s* restrict pitman, pit_timer_e pit_timerx)
{
  add_to_irq_v(IRQ_PIT, pitman->interrupt_callback);
  reset_pit(pitman, pit_timerx);
}

void
reset_pit(timer_manager_s* restrict pitman, pit_timer_e pit_timerx)
{
  // turn on PIT
  PIT_MCR_SET(MCR_RESET);
  // Push callback to interrupt vector
  pit_context_s* restrict pitctx = CONTEXT_TO_PIT(pitman->timer_ctx->context);

  switch (pitctx->freq)
  {
  case PIT_SPEED_50MHz:  
    PIT_LOADVAL0_SET(
        __resolve_time_50MHz__(pitman->targetval, pitman->time_type, FROMTIME_E));
    break;
  case PIT_SPEED_100MHz:
    PIT_LOADVAL0_SET(
        __resolve_time_100MHz__(pitman->targetval, pitman->time_type, FROMTIME_E));
    break;
  case PIT_SPEED_150MHz: 
    PIT_LOADVAL0_SET(
        __resolve_time_150MHz__(pitman->targetval, pitman->time_type, FROMTIME_E));
    break;
  case PIT_SPEED_200MHz:
    PIT_LOADVAL0_SET(
        __resolve_time_200MHz__(pitman->targetval, pitman->time_type, FROMTIME_E));
    break;
  }

  switch (pit_timerx)
  {
  case PIT_TIMER_00:
    PIT_TCTRL0_IRQ_EN(ENABLE);   // enable Timer 1 interrupts
    PIT_TCTRL0_TIMER_EN(ENABLE); // start Timer 1
    break;
  case PIT_TIMER_01:
    PIT_TCTRL1_IRQ_EN(ENABLE);   // enable Timer 2 interrupts
    PIT_TCTRL1_TIMER_EN(ENABLE); // start Timer 2
    break;
  case PIT_TIMER_02:
    PIT_TCTRL2_IRQ_EN(ENABLE);   // enable Timer 3 interrupts
    PIT_TCTRL2_TIMER_EN(ENABLE); // start Timer 3
    break;
  case PIT_TIMER_03:
    PIT_TCTRL3_IRQ_EN(ENABLE);   // enable Timer 4 interrupts
    PIT_TCTRL3_TIMER_EN(ENABLE); // start Timer 4
    break;    
  }
}
