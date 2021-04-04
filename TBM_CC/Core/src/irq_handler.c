#include "irq_handler.h"
//
// void
// attach_irq(uint8_t pin, void_func funcptr, int mode)
// {
//   if (pin >= CORE_NUM_DIGITAL)
//     return;
//   // printf("attachInterrupt, pin=%u\n", pin);
//   volatile uint32_t * gpio = portOutputRegister(pin);
//   volatile uint32_t * mux = portConfigRegister(pin);
//   // volatile uint32_t *pad = portControlRegister(pin);
//   uint32_t mask = digitalPinToBitMask(pin);
//
//   voidFuncPtr * table;
//
//   switch ((uint32_t)gpio) {
//     case (uint32_t)&GPIO6_DR: table = isr_table_gpio1; break;
//     case (uint32_t)&GPIO7_DR: table = isr_table_gpio2; break;
//     case (uint32_t)&GPIO8_DR: table = isr_table_gpio3; break;
//     case (uint32_t)&GPIO9_DR: table = isr_table_gpio4; break;
//     default: return;
//   }
//
//   add_to_irq_v(IRQ_GPIO6789, &irq_gpio6789);
//   NVIC_ENABLE_IRQ(IRQ_GPIO6789);
//
//   uint32_t icr;
//   switch (mode) {
//     case CHANGE: icr = 0; break;
//     case RISING: icr = 2; break;
//     case FALLING: icr = 3; break;
//     case LOW: icr = 0; break;
//     case HIGH: icr = 1; break;
//     default: return;
//   }
//
//   // TODO: global interrupt disable to protect these read-modify-write
//   accesses? gpio[IMR_INDEX] &= ~mask; // disable interrupt *mux = 5; // pin
//   is GPIO gpio[GDIR_INDEX] &= ~mask; // pin to input mode uint32_t index =
//   __builtin_ctz(mask); table[index] = function; if (mode == CHANGE) {
//     gpio[EDGE_INDEX] |= mask;
//   } else {
//     gpio[EDGE_INDEX] &= ~mask;
//     if (index < 16) {
//       uint32_t shift = index * 2;
//       gpio[ICR1_INDEX] = (gpio[ICR1_INDEX] & ~(3 << shift)) | (icr << shift);
//     } else {
//       uint32_t shift = (index - 16) * 2;
//       gpio[ICR2_INDEX] = (gpio[ICR2_INDEX] & ~(3 << shift)) | (icr << shift);
//     }
//   }
//   gpio[ISR_INDEX] = mask; // clear any prior pending interrupt
//   gpio[IMR_INDEX] |= mask; // enable interrupt
// }
//
// void
// detach_irq(uint8_t pin)
// {
//   if (pin >= CORE_NUM_DIGITAL)
//     return;
//   volatile uint32_t * gpio = portOutputRegister(pin);
//   uint32_t            mask = digitalPinToBitMask(pin);
//   gpio[IMR_INDEX] &= ~mask;
// }