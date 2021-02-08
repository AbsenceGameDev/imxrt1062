#ifndef IOMUX_MANAGER_H
#define IOMUX_MANAGER_H

#include "gpio_handler.h"
// #include "lcdif_handler.h"
// #include "qtimer_handler.h"
// #include "flexpwm_handler.h"
// #include "armtrace_handler.h"
// #include "flexio_handler.h"
// #include "srcboot_handler.h"
// #include "enet_handler.h"

typedef union {
  // SStoredDEVICE _device;
  // SStoredDEVICE _device;
  // SStoredDEVICE _device;
  // SStoredDEVICE _device;
  // SStoredDEVICE _device;
  SStoredGPIO gpio_device;
  // SStoredDEVICE _device;
  // SStoredDEVICE _device;
} iomux_manager;

#endif // IOMUX_MANAGER_H