#include "iomux_controller.h"

void
init_device_muxmode(SStoredMUXDevice * mux_device,
                    vuint32_t *        SW_MUX,
                    vuint32_t *        SW_PAD,
                    EBitMuxPad_DSE     DSE_OPT,
                    uint8_t            ctrl_pos,
                    EMuxMode           mux_mode)
{
  /** @brief: Set MUXmode, ALT5 = GPIO */
  *((mux_device->mux_pad_context.mux_pad_addr = SW_MUX) +
    (mux_device->ctrl_pos = ctrl_pos)) = (mux_device->mux_mode = mux_mode);

  /** @brief: Set DSE field (Drive Strength Field) */
  *((mux_device->mux_pad_context.pad_pad_addr = SW_PAD) + ctrl_pos) =
      IOMUXC_PAD_DSE(DSE_OPT);
}

void
set_mux_mode(SStoredMUXDevice * mux_device, EMuxMode mux_mode)
{
  *(mux_device->mux_pad_context.mux_pad_addr) = mux_mode;
  return;
}