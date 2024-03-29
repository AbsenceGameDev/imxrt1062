//#include "../include/iomux_controller.h"
#include "iomux_controller.h"
void
init_device_muxmode(SStoredMUXDevice * mux_device,
                    vuint32_t *        sw_mux,
                    vuint32_t *        sw_pad,
                    EBitMuxPad_DSE     dse_opt,
                    uint8_t            ctrl_pos,
                    EMuxMode           mux_mode)
{
  if (mux_device == NULL) {
    mux_device = (SStoredMUXDevice *)malloc_(MUXDEVICE_BYTESIZE);
    if (mux_device == NULL) {
      return;
    }
  }
  /** @brief: Set MUXmode */
  *((mux_device->mux_pad_context.mux_pad_addr = sw_mux) +
    (mux_device->ctrl_pos = ctrl_pos)) = (mux_device->mux_mode = mux_mode);

  /** @brief: Set DSE field (Drive Strength Field) */
  *((mux_device->mux_pad_context.pad_pad_addr = sw_pad) + ctrl_pos) =
      IOMUXC_PAD_DSE(dse_opt);
}

void
set_muxmode(SStoredMUXDevice * mux_device, EMuxMode mux_mode)
{
  *(mux_device->mux_pad_context.mux_pad_addr) = mux_mode;
}