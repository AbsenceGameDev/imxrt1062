#include "display_driver.h"

// Abstracted layer
ssd1306_intf *
ssd1306_create_interface()
{
  return ((ssd1306_intf *)0); // return null, placeholder
}

void
ssd1306_set_address_mode(EMemAddressMode memory_mode)
{
  ssd1306_send_command_byte(0x20);
  ssd1306_send_data_byte(memory_mode);
}

void
ssd1306_set_command_register(ECommandRegisters cmd_hex, unsigned char value)
{
  ssd1306_send_command_byte(cmd_hex);
  ssd1306_send_data_byte(value);
}

// Physical layer
static void
ssd1306_phy_send_byte(unsigned char data, trigger_gpio_fp_t trigger_gpio)
{

  unsigned char i;
  for (i = 8; i > 0; i--) {
    // Send write-bit pulse
    trigger_gpio((vuint32_t *)0, i); // null addr is placeholder
  }
  // generating confirmation impulse
}

static void
ssd1306_phy_send_bytes(const unsigned char * buf,
                       unsigned short        size,
                       trigger_gpio_fp_t     trigger_gpio)
{
  while (size--) {
    ssd1306_phy_send_byte(*buf, trigger_gpio);
    ++buf;
  }
}

/** SCL remains HIGH on EXIT, Low SDA means start transmission */
static void
ssd1306_phy_start(void)
{
}

static void
ssd1306_phy_stop(void)
{
}

static void
ssd1306_phy_close()
{
}

void
ssd1306_phy_init(char scl, char sda, char sa)
{
  // Placeholder/dummy body to get armgcc to compile right now
  const unsigned char buf_dummy[] = "1234\0";
  unsigned short      size = 0;
  trigger_gpio_fp_t   temp = ((trigger_gpio_fp_t)0);
  ssd1306_phy_start();
  ssd1306_phy_send_bytes(buf_dummy, size, temp);
  ssd1306_phy_stop();
  ssd1306_phy_close();
}
