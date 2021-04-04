#ifndef CLK_CONTROL_H
#define CLK_CONTROL_H

#include "system_memory_map.h"

typedef enum
{
  IPG_ROOT = 0x0,
  OSC_ROOT = 0x1
} perclk_src_e;
#define CCM_SCMUX1_SET(x) CCM_C_SCMR1 = (x)
#define CCM_SCMUX1_DIV(x) (x & 0b111111)
#define CCM_SCMUX1_DIV_SET(x)                                                  \
  CCM_C_SCMR1 = ((CCM_C_SCMR1 & ~0b111111) | CCM_SCMUX1_DIV(x))
#define CCM_PERCLK_SRC(x) ((x & 0x1) << 0x6)
#define CCM_SLCT_PERCLK_SRC(x)                                                 \
  CCM_C_SCMR1 = ((CCM_C_SCMR1 & ~(0x1 << 0x6)) | CCM_PERCLK_SRC(x))

#endif // CLK_CONTROL_H