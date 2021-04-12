#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

#include "system_memory_map.h"

/**
 *  @brief 4.3 CM7 interrupts (p.43 - p.52), IMXRT1060 Processor Ref Manual
 *
 *  @details Due to the clock frequency difference between CPU andperipheral, in
 *           some corner case, peripheral interrupt flag maynot be really
 *           cleared before CPU exit ISR. In such case, user should add DSB
 *           instruction right after instruction to clear interrupt flag.
 *
 **/
typedef enum
{
  // CM7
  IRQ_CM7_CTI0 = 17,
  IRQ_CM7_CTI1 = 18,
  IRQ_CM7_CORE_ERROR = 19,
  IRQ_CM7_ADDR_ERR = 38, /** Out of bounds or access hit */

  // DMA
  IRQ_DMA_CH0_TX = 0,
  IRQ_DMA_CH1_TX = 1,
  IRQ_DMA_CH2_TX = 2,
  IRQ_DMA_CH3_TX = 3,
  IRQ_DMA_CH4_TX = 4,
  IRQ_DMA_CH5_TX = 5,
  IRQ_DMA_CH6_TX = 6,
  IRQ_DMA_CH7_TX = 7,
  IRQ_DMA_CH8_TX = 8,
  IRQ_DMA_CH9_TX = 9,
  IRQ_DMA_CH10_TX = 10,
  IRQ_DMA_CH11_TX = 11,
  IRQ_DMA_CH12_TX = 12,
  IRQ_DMA_CH13_TX = 13,
  IRQ_DMA_CH14_TX = 14,
  IRQ_DMA_CH15_TX = 15,
  IRQ_DMA_ERROR = 16,

  // LPUART
  IRQ_LPUART1_TRX = 20, /** RX or TX irq */
  IRQ_LPUART2_TRX = 21, /** RX or TX irq */
  IRQ_LPUART3_TRX = 22, /** RX or TX irq */
  IRQ_LPUART4_TRX = 23, /** RX or TX irq */
  IRQ_LPUART5_TRX = 24, /** RX or TX irq */
  IRQ_LPUART6_TRX = 25, /** RX or TX irq */
  IRQ_LPUART7_TRX = 26, /** RX or TX irq */
  IRQ_LPUART8_TRX = 27, /** RX or TX irq */

  // LPI²C
  IRQ_LPI2C1 = 28, /** either master or slave, async or not */
  IRQ_LPI2C2 = 29, /** either master or slave, async or not */
  IRQ_LPI2C3 = 30, /** either master or slave, async or not */
  IRQ_LPI2C4 = 31, /** either master or slave, async or not */
  IRQ_LPSPI1 = 32, /** interrupt request line to the core */
  IRQ_LPSPI2 = 33, /** interrupt request line to the core */
  IRQ_LPSPI3 = 34, /** interrupt request line to the core */
  IRQ_LPSPI4 = 35, /** interrupt request line to the core */

  // FLEXCAN
  IRQ_FLEXCAN1 = 36,
  IRQ_FLEXCAN2 = 37,
  IRQ_FLEXCAN3 = 154,
  /** Combined interrupt of ini_int_busoff, ini_int_error,ipi_int_mbor,
         ipi_int_rxwarning, ipi_int_txwarning andipi_int_wakein.*/

  // SAI
  IRQ_SAI1 = 56, /** Interrupt for SAI1, RX or TX, async or not */
  IRQ_SAI2 = 57, /** Interrupt for SAI2, RX or TX, async or not */
  IRQ_SAI3_RX = 58, /** Interrupt for SAI3 RX, async or not */
  IRQ_SAI3_TX = 59, /** Interrupt for SAI3 TX, async or not */

  // GPIO
  IRQ_GPIO1_INT0 = 72,
  IRQ_GPIO1_INT1 = 73,
  IRQ_GPIO1_INT2 = 74,
  IRQ_GPIO1_INT3 = 75,
  IRQ_GPIO1_INT4 = 76,
  IRQ_GPIO1_INT5 = 77,
  IRQ_GPIO1_INT6 = 78,
  IRQ_GPIO1_INT7 = 79,
  IRQ_GPIO1_0_15 = 80,
  IRQ_GPIO1_16_31 = 81,
  IRQ_GPIO2_0_15 = 82,
  IRQ_GPIO2_16_31 = 83,
  IRQ_GPIO3_0_15 = 84,
  IRQ_GPIO3_16_31 = 85,
  IRQ_GPIO4_0_15 = 86,
  IRQ_GPIO4_16_31 = 87,
  IRQ_GPIO5_0_15 = 88,
  IRQ_GPIO5_16_31 = 89,
  IRQ_GPIO6789 = 157,

  // Analog Modules
  IRQ_TSC_DIG = 40, /** TSC interrupt */
  IRQ_ADC1 = 67, /** @todo Continue commenting later. */
  IRQ_ADC2 = 68,
  IRQ_ADC_ETC0 = 118,
  IRQ_ADC_ETC1 = 119,
  IRQ_ADC_ETC2 = 120,
  IRQ_ADC_ETC_ERR = 121,
  IRQ_ACMP1 = 123,
  IRQ_ACMP2 = 124,
  IRQ_ACMP3 = 125,
  IRQ_ACMP4 = 126,

  // CLOCKS & TIMERS
  IRQ_WDOG2 = 45, /** Watchdog Timer reset */
  IRQ_CCM1 = 95,
  IRQ_CCM2 = 96,
  IRQ_WDOG1 = 92,
  IRQ_RTWDOG = 93,
  IRQ_PIT = 122,
  IRQ_QTIMER1 = 133,
  IRQ_QTIMER2 = 134,
  IRQ_QTIMER3 = 135,
  IRQ_QTIMER4 = 136,
  IRQ_GPT1 = 100,
  IRQ_GPT2 = 101,

  // FLEXPMV
  IRQ_FLEXPWM1_0 = 102,
  IRQ_FLEXPWM1_1 = 103,
  IRQ_FLEXPWM1_2 = 104,
  IRQ_FLEXPWM1_3 = 105,
  IRQ_FLEXPWM1_FAULT = 106,
  IRQ_FLEXPWM2_0 = 137,
  IRQ_FLEXPWM2_1 = 138,
  IRQ_FLEXPWM2_2 = 139,
  IRQ_FLEXPWM2_3 = 140,
  IRQ_FLEXPWM2_FAULT = 141,
  IRQ_FLEXPWM3_0 = 142,
  IRQ_FLEXPWM3_1 = 143,
  IRQ_FLEXPWM3_2 = 144,
  IRQ_FLEXPWM3_3 = 145,
  IRQ_FLEXPWM3_FAULT = 146,
  IRQ_FLEXPWM4_0 = 147,
  IRQ_FLEXPWM4_1 = 148,
  IRQ_FLEXPWM4_2 = 149,
  IRQ_FLEXPWM4_3 = 150,
  IRQ_FLEXPWM4_FAULT = 151,

  // ENET
  IRQ_ENET = 114,
  IRQ_ENET_TIMER = 115,
  IRQ_ENET2_MAC0 = 152,
  IRQ_ENET2_TIMER_SYNC = 153,

  // MISC
  IRQ_KPAD = 39, /** Keypad interrupt */
  IRQ_GPR_IRQ = 41, /** Used to notify cores on exception cond. while boot */
  IRQ_LCDIF = 42, /** LCDIF Sync Interrupt */
  IRQ_CSI = 43, /** CSI Interrupt */
  IRQ_PXP = 44, /** PXP Interrupt */
  IRQ_SNVS_IRQ = 46, /** SVNS Functional Interrupt */
  IRQ_SNVS_SECURITY = 47, /** SVNS Security Interrupt */
  IRQ_SNVS_ONOFF_5SEC = 48, /** ON-OFF btn press < 5 seconds (pulse event) */
  IRQ_CSU = 49, /** CSU Interrupt Request 1, indicates to the core that 1 or
                   more alarm inputs were asserted */
  IRQ_DCP0 = 50, /** Combined DCP ch. irq's (except ch. 0) and CRC irq */
  IRQ_DCP1 = 51, /** IRQ DCP ch. 0 */
  IRQ_DCP2 = 52, /** Reserved */
  IRQ_TRNG = 53, /** True-RNG Interrupt */
  IRQ_BEE = 55, /** BEE Interrupt */
  IRQ_SPDIF_TRX = 60, /** SPDIF RX or TX Interrupt */
  IRQ_PMU_BROWNOUT0 = 61, /** Brownout on either 1.1, 2.5 or 3.0 regulators */
  IRQ_TEMPERATURE = 63, /** Low or High temp IRQ */
  IRQ_TEMPERATURE_PANIC = 64, /** Panic temperature IRQ (High or Low) */
  IRQ_USBPHY0 = 65, /** UTMI0 Interrupt */
  IRQ_USBPHY1 = 66, /** UTMI1 Interrupt */
  IRQ_DCDC = 69,
  IRQ_FLEXIO1 = 90,
  IRQ_FLEXIO2 = 91,
  IRQ_FLEXIO3 = 156,
  IRQ_EWM = 94,
  IRQ_GPC = 97,
  IRQ_SRC = 98,
  IRQ_FLEXSPI2 = 107,
  IRQ_FLEXSPI = 108,
  IRQ_SEMC = 109,
  IRQ_SDHC1 = 110,
  IRQ_SDHC2 = 111,
  IRQ_USB2 = 112,
  IRQ_USB1 = 113,
  IRQ_XBAR1_01 = 116,
  IRQ_XBAR1_23 = 117,
  IRQ_ENC1 = 129,
  IRQ_ENC2 = 130,
  IRQ_ENC3 = 131,
  IRQ_ENC4 = 132,

  // Reserved
  IRQ_RESERVED0 = 54, /** Reserved */
  IRQ_RESERVED1 = 62, /** Reserved */
  IRQ_RESERVED2 = 70,
  IRQ_RESERVED3 = 71,
  IRQ_RESERVED4 = 99,
  IRQ_RESERVED5 = 127,
  IRQ_RESERVED6 = 128,
  IRQ_RESERVED7 = 155,
  IRQ_RESERVED8 = 158,
  IRQ_RESERVED9 = 159,

} irq_num_e;

typedef void (*void_func)(void);
extern void (*__vectors_ram__[NVIC_IRQs + 0x10])(void);

static inline void
add_to_irq_v(irq_num_e irq, void_func funcptr)
    __attribute__((always_inline, unused));

static inline void
add_to_irq_v(irq_num_e irq, void_func funcptr)
{
  __vectors_ram__[irq + 0x10] = funcptr;
  asm volatile("" : : : "memory");
}

static inline void
remove_from_irq_v(irq_num_e irq) __attribute__((always_inline, unused));
static inline void
remove_from_irq_v(irq_num_e irq)
{
  __vectors_ram__[irq + 0x10] = ((void_func)0);
  asm volatile("" : : : "memory");
}

#endif // IRQ_HANDLER_H