/**
 * @file      iomux_controller.c
 * @author    Ario@Permadev
 * @brief
 * @version   0.1
 * @date      2021-08-29
 *
 * @copyright Copyright (c) 2021, MIT-License included in project toplevel dir
 *
 */

#include "iomux_controller.h"
void
init_device_muxmode(muxdev_s *   mux_device,
                    vuint32_t *  sw_mux,
                    vuint32_t *  sw_pad,
                    muxpad_dse_e dse_opt,
                    uint8_t      ctrl_pos,
                    muxmode_e    mux_mode)
{
  mux_device = (muxdev_s *)malloc_(sizeof(muxdev_s));
  if (mux_device == NULL) {
    return;
  }

  /** @brief: Set MUXmode */
  *((mux_device->mux_pad_context.mux_pad_addr = sw_mux) +
    (mux_device->ctrl_pos = ctrl_pos)) = (mux_device->mux_mode = mux_mode);

  // if (ctrl_pos == 0x3) {
  // }

  /** @brief: Set DSE field (Drive Strength Field) */
  *((mux_device->mux_pad_context.pad_pad_addr = sw_pad) + ctrl_pos) =
      IOMUXC_PAD_DSE(dse_opt);
}

void
set_muxmode(muxdev_s * mux_device, muxmode_e mux_mode)
{
  *(mux_device->mux_pad_context.mux_pad_addr) = mux_mode;
}