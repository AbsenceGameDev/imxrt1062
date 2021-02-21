#include "display_driver.h"

void
ssd1306_set_address_mode(EMemAddressMode memory_mode)
{
  send_command_byte(0x20);
  send_data_byte(memory_mode);
}