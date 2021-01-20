#include "gpio_handler.h"

EAnalogState
read_state_32t_DataPin(vuint32_t * pin_addr){};

EAnalogState
read_state_16t_DataPin(vuint16_t * pin_addr){};

EAnalogState
read_state_8t_DataPin(vuint8_t * pin_addr){};

typedef union {
  EBitMuxPad_SRE   slew_rate;
  EBitMuxPad_DSE   drive_strength;
  EBitMuxPad_SPEED speed;
  EBitMuxPad_ODE   open_drain;
  EBitMuxPad_PKE   pull_keep_enable;
  EBitMuxPad_PUE   pull_keep_select;
  EBitMuxPad_PUS   pull_up_down_conf;
  EBitMuxPad_HYS   hysteresis_enable;
  EMuxMode         selected_mux_mode;
} UPadFields;

typedef struct {
  vuint32_t * mux_pad_addr;
  vuint32_t * pad_pad_addr;
  UPadFields  current_pad_type;
} SPadContext;

/**
 * TODO: Go through documentations to see what how the iomuxc pads are
 * configured, also check the bootdata.c, startup.c and imxrt1062.ld too see if
 * the iomux pads are configured there
 *
 * @brief: The imxrt1062 has 9 GPIOs, each of them has 32 channels? Not sure
 * will have to read more in the reference manual
 *
 * High-speed GPIOs exist in this device:
 * •  GPIO 1-5 are standard-speed GPIOs that run off the IPG_CLK_ROOT,
 *    while GPIO 6-9 are high-speed GPIOs that run at the AHB_CLK_ROOT freq.
 *    See the table "System Clocks, Gating, and Override" in CCM chapter.
 *
 * •  Regular GPIO and high speed GPIO are paired
 *    (GPIO1 and GPIO6 share the same pins, GPIO2 and GPIO7 share, etc).
 *    The IOMUXC_GPR_GPR26-29 registers areused to determine if the regular or
 *    high-speed GPIO module is used for the GPIO pins on a given port.
 *
 * NOTE: / TODO:
 *  Read through chapter 10-12 tomorrow, everything from overview to functioncal
 *  descriptions. Need to understand how they work now that I have a small
 *  understanding of what they are
 *
 * Maybe change name of pad_position to control_position?
 *
 * NOTE: The programming sequence for driving output signals should be:
 *
 * 1.  Configure IOMUX to select GPIO mode (Via IOMUXC), also enable SION if
 *     need to read loopback pad value through PSR
 * 2.  Configure GPIO direction register to output (GPIO_GDIR[GDIR] set to 1b).
 * 3.  Write value to data register (GPIO_DR).
 *
 * A pseudocode description to drive 4'b0101 on [output3:output0] is as follows:
 * // SET PADS TO GPIO MODE VIA IOMUX.
 * write sw_mux_ctl_pad_<output[0-3]>.mux_mode, <GPIO_MUX_MODE>
 *
 * // Enable loopback so we can capture pad value into PSR in output mode
 * write sw_mux_ctl_pad_<output[0-3]>.sion, 1
 *
 * // SET GDIR=1 TO OUTPUT BITS.
 * write GDIR[31:4,output3_bit,output2_bit, output1_bit, output0_bit,]
 *32'hxxxxxxxF
 *
 * // WRITE OUTPUT VALUE=4’b0101 TO DR.
 * write DR, 32'hxxxxxxx5
 *
 * // READ OUTPUT VALUE FROM PSR ONLY.
 * read_cmp PSR, 32'hxxxxxxx5
 *
 **/
inline void
init_helper(vuint32_t *    SW_MUX,
            vuint32_t *    SW_PAD,
            EBitMuxPad_DSE DSE_OPT,
            uint8_t        control_position)
{
  *(SW_MUX + control_position) = 0x5;

  /** @brief: Set DSE field (Drive Strength Field) */
  *(SW_PAD + control_position) = IOMUXC_PAD_DSE(DSE_OPT);
}

void
init_gpio(SStoredGPIO    gpio_device,
          EBaseGPIO      gpio_register,
          EPadCR         pad_group,
          uint8_t        control_position,
          EBitMuxPad_DSE DSE_OPT)
{
  switch (pad_position) {
    case GPIO_AD_B0:
      /** @brief: Set MUXmode, ALT5 = GPIO1_IOx x: [0,15] (ctrl_postion + 1) */
      init_helper(IOMUXC_MUX_PAD_GPIO_AD_B0_CR00,
                  IOMUXC_PAD_PAD_GPIO_AD_B0_CR00,
                  DSE_OPT,
                  control_position);
      break;
    case GPIO_AD_B1:
      /** @brief: Set MUXmode, ALT5 = GPIO1_IOx x: [16,31] (ctrl_postion + 1) */
      init_helper(IOMUXC_MUX_PAD_GPIO_AD_B1_CR00,
                  IOMUXC_PAD_PAD_GPIO_AD_B1_CR00,
                  DSE_OPT,
                  control_position);
      break;
    case GPIO_B0:
      /** @brief: Set MUXmode, ALT5 = GPIO2_IOx x: [0,15] (ctrl_postion + 1) */
      *((&IOMUXC_MUX_PAD_GPIO_B0_CR00) + control_position) = 0x5;
      init_helper(IOMUXC_MUX_PAD_GPIO_B0_CR00,
                  IOMUXC_PAD_PAD_GPIO_B0_CR00,
                  DSE_OPT,
                  control_position);
      break;
    case GPIO_B1:
      /** @brief: Set MUXmode, ALT5 = GPIO2_IOx x: [16,31] (ctrl_postion + 1) */
      init_helper(IOMUXC_MUX_PAD_GPIO_B1_CR00,
                  IOMUXC_PAD_PAD_GPIO_B1_CR00,
                  DSE_OPT,
                  control_position);
      break;
    case GPIO_SD_B0:
      /** @brief: Set MUXmode, ALT5 = GPIO1_IOx x: [12,17] (ctrl_position + 1) */
      init_helper(IOMUXC_MUX_PAD_GPIO_SD_B0_CR00,
                  IOMUXC_PAD_PAD_GPIO_SD_B0_CR00,
                  DSE_OPT,
                  ctrl_position);
      break;
    case GPIO_SD_B1:
      /** @brief: Set MUXmode, ALT5 = GPIO1_IOx x: [0,11] (ctrl_position + 1) */
      init_helper(IOMUXC_MUX_PAD_GPIO_SD_B1_CR00,
                  IOMUXC_PAD_PAD_GPIO_SD_B1_CR00,
                  DSE_OPT,
                  ctrl_position);
      break;
  }

  /**
   * @brief: Set all MUX bits in the correct General Purpose Register (GPR)
   **/
  switch (gpio_device.pin) {
      // GPR26 [GPIO1,GPIO6]
    case 0x1:
    case 0x6:
      //  Set all MUX bits to either GPIO1 or GPIO6
      IOMUXC_GPR_GPR26 = 0xffffffff * (gpio_device.pin == 0x6);
      break;
      // GPR27 [GPIO2,GPIO7]
    case 0x2:
    case 0x7:
      // Set all MUX bits to either GPIO2 or GPIO7
      IOMUXC_GPR_GPR27 = 0xffffffff * (gpio_device.pin == 0x7);
      break;
      // GPR28 [GPIO3,GPIO8]
    case 0x3:
    case 0x8:
      // Set all MUX bits to either GPIO3 or GPIO8
      IOMUXC_GPR_GPR28 = 0xffffffff * (gpio_device.pin == 0x8);
      break;
      // GPR29 [GPIO4,GPIO9]
    case 0x4:
    case 0x9:
      // Set all MUX bits to either GPIO4 or GPIO9
      IOMUXC_GPR_GPR29 = 0xffffffff * (gpio_device.pin == 0x9);
      break;
  }
  uint_fast8_t dir = 0x3;
};

void *
handle_gpio(SStoredGPIO gpio_device, EBaseGPIO gpio_register)
{
  vuint32_t * gpio_ptr = *(gpio_device.base_addr + gpio_register);
  switch (gpio_register) {

    case GDIR_DIR_REG:
      *gpio_ptr |= (0x1 << gpio_device.bit_id); // Set as OUTPUT
      return NULL;
    case PSR_PAD_STATUS_REG:
      return 0x3 | ((*gpio_ptr) >> gpio_device.bit_id); // Read Only
    case DR_SET: // WO
    case DR_CLEAR: // WO
    case DR_TOGGLE: // WO
      *gpio_ptr = (0x1 << gpio_device.bit_id);
      return NULL;
    case ICR1_INTERRUPT_CONF_REG1: // regards GPIO [0,15]
      *gpio_ptr = (0x1 << gpio_device.bit_id);
      return NULL;
    case ICR2_INTERRUPT_CONF_REG2: // regards GPIO [16,31]
      *gpio_ptr = (0x1 << gpio_device.bit_id);
      return NULL;
    case IMR_INTERRUPT_MASK_REG: return NULL;
    case ISR_INTERRUPT_STAT_REG: return NULL;
  }
  return NULL;
};

void
set_icr1(SStoredGPIO * gpio_device, E_ICRFIELDS_GPIO setting)
{
  *(gpio_device->base_addr + ICR1_INTERRUPT_CONF_REG1) = setting;
};

void
set_icr2(SStoredGPIO * gpio_device, E_ICRFIELDS_GPIO setting)
{
  *(gpio_device->base_addr + ICR2_INTERRUPT_CONF_REG2) = setting;
};

// E_ICRFIELDS_GPIO

uint32_t
read_gpio(vuint32_t gpio_base_addr, EBaseGPIO gpio_register)
{
  return *(gpio_base_addr + gpio_register);
};

void
set_gpio_muxmode(vuint32_t * gpio_addr, EMuxModes mux_mode)
{
  *gpio_addr = mux_mode;
}

void
set_gpio_gdir(vuint32_t * gpio_gdir_addr, uint_fast8_t direction_bit)
{
  *gpio_gdir_addr |= (0x1 << direction_bit);
}

void
set_gpio_datar(vuint32_t * gpio_dr_set_addr, uint_fast8_t direction_bit)
{
  *gpio_dr_set_addr = (0x1 << direction_bit);
}

void
clr_gpio_datar(vuint32_t * gpio_dr_clr, uint_fast8_t direction_bit)
{
  *gpio_dr_clr = (0x1 << direction_bit);
}

void
tog_gpio_datar(vuint32_t * gpio_dr_tog, uint_fast8_t direction_bit)
{
  *gpio_dr_tog = (0x1 << direction_bit);
}

void
set_iomuxc_byte(vuint32_t * addr, uint_fast8_t byte)
{
  *addr = byte;
}
void
set_iomuxc_word(vuint32_t * addr, uint_fast16_t word)
{
  *addr = word;
}
void
set_iomuxc_dword(vuint32_t * addr, uint_fast32_t dword)
{
  *addr = dword;
}

void
flip_selected_gpr(vuint32_t gpr_iomuxc_gpr)
{
  *gpr_iomuxc_gpr = !(gpr_iomuxc_gpr);
}

void
set_iomuxc_gpr(vuint32_t gpr_iomuxc_gpr, EState set_state)
{
  (*gpr_iomuxc_gpr) = 0xffffffff * (set_state);
}

/**
 * @brief: Blinky LED Example
 * Configure GPIO B0_03 (PIN 13) for output, ALT 5 according to p.511 */
void
blinky_led_example()
{
  IOMUXC_MUX_PAD_GPIO_B0_CR03 = 0x5;
  IOMUXC_PAD_PAD_GPIO_B0_CR03 = IOMUXC_PAD_DSE(0x7);

  // GPR27, Set it to Control
  IOMUXC_GPR_GPR27 = 0xffffffff;

  // Seems like the dir offset is related to the control registers in the iomuxc
  // above, will have to look into it more tomorrow
  uint_fast8_t dir = 0x3;

  /** GPIO_GDIR functions as direction control when the IOMUXC is in GPIO mode.
   *  Each bit specifies the direction of a one-bit signal.
   **/
  // Set DPIO7 direction (set as output = 1, input = 0), in GDIR
  set_gpio_gdir(GPIO7_DIRR, dir);

  for (;;) {
    volatile unsigned int i = 0x0;

    // Set PIN 13 HIGH,
    set_gpio_datar(GPIO7_DR_SET, dir);

    // Poor man's delay
    while (i < 0x10000) {
      i++;
    }

    i = 0;

    // Set PIN 13 LOW
    clr_gpio_datar(GPIO7_DR_CLEAR, dir);

    // Poor man's delay
    while (i < 0x010000) {
      i++;
    }
  }
}
