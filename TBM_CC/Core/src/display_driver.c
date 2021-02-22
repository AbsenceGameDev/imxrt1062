#include "display_driver.h"

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

static void
ssd1306_phy_send_byte(unsigned char data)
{
  unsigned char i;
  for (i = 8; i > 0; i--) {
    // Send write-bit pulse
  }
  // generating confirmation impulse
}

static void
ssd1306_phy_send_bytes(const unsigned char * buf, unsigned short size)
{
  while (size--) {
    ssd1306_phy_send_byte(*buf);
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
}
