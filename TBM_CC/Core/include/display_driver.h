#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include "system_memory_map.h"

/**
 * @brief 128x32 MINIOLED
 * OLED Driver IC: SSD1306 (SOLOMON SYSTECH)
 * Resolution: 128 x 32
 * Visual Angle: >160°
 * Input Voltage: 3.3V ~ 5V
 * Compatible I/O Level: 3.3V, 5V
 * Only Need 2 I/O Port to Control
 * Relevant pins: GND, VCC, SCL, SDA
 * GND = Ground;
 * VCC = Voltage input;
 * SCL = I2C-bus clock signal The transmission of information in the I2C-bus is
 *       following a clock signal, SCL. Each transmission of data bit is taken
 *       place during a single clock period of SCL.
 *       SCL - START CONDITION -> Falling Edge
 *       SCL - STOP CONDITION -> RISING Edge
 *
 * SDA = I2C-bus data signal, SDA acts as a communication channel between the
 *       transmitter and the receiver.
 *       The data and the acknowledgement are sent through the SDA.It should be
 *       noticed that the ITO track resistance and the pulled-up resistance at
 *       “SDA” pin becomes a voltage potential divider.
 *       As a result, the acknowledgement would not be possible to attain a
 *       valid logic 0 level in “SDA”.
 *       “SDAIN” and “SDAOUT” are tied together and serve as SDA.
 *       The “SDAIN” pin must be connected to act as SDA.
 *       The “SDAOUT” pin may be disconnected. When “SDAOUT” pin is
 *       disconnected, the acknowledgement signal'll be ignored in the I2C-bus.
 *
 * THOUGHTS:
 *  First I need to power on the lcd with the above mentioned power-onn
 *  sequence. Then after it is powered on I send a boot up command, from there I
 *  need to set some initializing commands.
 *
 *  Then to test, try sending A4h/A5h commands, A5h forces entire display ON,
 *  while A4h resumes the display from the entire display "ON" stage. (p.37)
 *
 *  As noted above, set the addressing mode (page-addressing preferrably)
 *  And perform it's neccessary setup steps
 *
 *  When data needs to be sent to the screen I must also deliver a signal
 *  through SCL input to triger the clock. The clock needs to be signalled
 *  before data from SDA can be read.
 *
 * Now the question remains how the data is actually being read, and if I can
 *  gain direct access to the screen through the SDA pin somehow.
 *
 *  Ok, looking at the actual driverboard;
 *  it looks like the SDA circuit leads into R1 resistor.
 *  R1, R2 & R3 are connected in series, followed by D1 which is followed by C8
 *  This means we have this chain of components -
 *  [SDA -> R1 -> R2 -> R3 -> D1 -> C8]
 *
 * Regarding SCL and SDA for data comms -
 *  Read chapter 8.1.5 (p.19-21) MINIOLED_SSD1206.pdf
 *  8.1.5 MCU I2C Interface -
 *
 * Regarding whether input is data or commands -
 *  Read chapter 8.2 (p.22) MINIOLED_SSD1206.pdf
 *  Read chapter 9. (p.28-33) MINIOLED_SSD1206.pdf (Commands)
 *  Read chapter 10. (p.34-46) MINIOLED_SSD1206.pdf (Commands)
 *
 * Regarding internal GDDRAM -
 *  Read chapter 8.7 (p.25) MINIOLED_SSD1206.pdf
 * - 8.7 Graphic Display Data RAM (GDDRAM)
 *       The GDDRAM is a bit mapped static RAM holding the bit pattern to be
 *       displayed. The size of the RAM is 128 x 64 are used for monochrome
 *       128x64 dot matrix display, as shown in Figure 8-13.
 *
 *       When one data byte is written into GDDRAM, all the rows image data of
 *       the same page of the current column are filled (i.e. the whole column
 *       (8 bits) pointed by the column address pointer is filled.). Data bit D0
 *       is written into the top row, while data bit D7 is written into bottom
 *       row as shown in Figure 8-14.
 *
 *       Memory Pages Structure, 2 settings
 *
 *       (Default setting):
 *       (SEG_0  <--> SEG_127)
 *
 *       PAGE_0 = COM0-COM7
 *       PAGE_1 = COM8-COM15
 *       PAGE_2 = COM16-COM23
 *       PAGE_3 = COM24-COM31
 *       PAGE_4 = COM32-COM39
 *       PAGE_5 = COM40-COM47
 *       PAGE_6 = COM48-COM55
 *       PAGE_7 = CO56-COM63
 *
 *       (Remapped Setting):
 *       (SEG_127  <--> SEG_0)
 *
 *       PAGE_0 = COM63-COM56
 *       PAGE_1 = COM55-COM48
 *       PAGE_2 = COM47-COM40
 *       PAGE_3 = COM39-COM32
 *       PAGE_4 = COM31-COM24
 *       PAGE_5 = COM23-COM16
 *       PAGE_6 = COM15-COM08
 *       PAGE_7 = CO07-COM00
 *
 * Regarding Powering On/Off -
 *  Read chapter 8.9 (p.27) MINIOLED_SSD1206.pdf
 *  Power ON -
 *  1. Power ON VDD
 *  2. After VDD become stable, set RES# pin LOW (logic low) for at least 3us
 *     (t1) (4) and then HIGH (logic high).
 *  3. After set RES# pin LOW (logic low), wait for at least 3us (t2). Then
 *     Power ON VCC.(1)
 *  4. After VCC become stable, send command AFh for display ON. SEG/COM
 *     will be ON after 100ms (tAF).
 *
 *  Power OFF -
 *  1. Send command AEh for display OFF.
 *  2. Power OFF VCC.(1), (2), (3)
 *  3. Power OFF VDD after tOFF. (5) (Typical tOFF=100ms)
 *
 *
 * IICBUS: I2C-Bus data format:
 * [S, Slave-Addr, m(CtrlByte, DataByte, ...), CtrlByte, n(DataByte, ...), P]
 *      Where:
 *      S = Start Condition
 *      P = Stop Condition
 *      m >= 0 words (ControlByte, DataByte)
 *      n >= 0 words (DataByte)
 *      Slave-Addr (if SA0 is LOW) = [0, 1, 1, 1, 1, 0, SA0, RW#]
 *      CtrlByte =   [Co, D/C#, 0, 0, 0, 0, 0, 0][ACK_BIT_HEADER]
 *
 *          Where:
 *          SA0 -> LOW == 0, HIGH == 1
 *          RW# -> 0 == Establish WriteMode
 *          if sent a control byte:
 *              Co -> 0 == Notify of Only data transmission
 *              D/C# -> next byte: {LOW (CMD) == 0, or HIGH (GDDRAM_DATA) == 1}
 *
 *      Start Condition: Established by pulling the SDA from HIGH to LOW
 *                       while the SCL stays HIGH.
 *
 *      Stop Condition: Established by pulling the “SDA in” from LOW to
 *                      HIGH while the “SCL” stays HIGH.
 *
 * NOTE: Write mode will finish when a stop condition is applied.
 *
 *  From reading the specs and looking at the driver-board I gather that I need
 *  to do this in approx. this proposed order:
 *
 *  0. Set Ground pin.
 *  1. Power ON VDD.
 *     (By setting VCC to min 7v Max 15v; we get VDD=2.8v if VCC is 12v)
 *  2. Send Start Condition (As described above)
 *  3. Send data according to the I2C Bus (As described above @IICBUS:)
 *      3a. Send 8 bits, [0,1,1,1,1,0][SA0=LOW][RW#=0]
 *      3b.After transmission of the slave address, send Ctrl-byte
 *         Ctrl byte =   [Co, D/C#, 0, 0, 0, 0, 0, 0]
 *         Co -> 0 == Expect only Data bytes, 1 == Expect more Ctrl bytes
 *         D/C# -> next byte: {LOW (CMD) == 0, or HIGH (GDDRAM_DATA) == 1}
 *      (Incoming databytes set to read as GDDRAM DATA, will increment through
 *      the GDDRAM according to the current GDDRAM Page Addressing Mode)
 *
 *
 *  4. Repeat step 3 for as long as needed.
 *
 *  5. Send Command 0xAE, to power off display.
 *  6. Send Stop Condition.
 *  7. Power OFF VCC
 *  8. Power OFF VDD after tOFF. (Typical tOFF=100ms)
 *
 * Read the command list and see which commands might be usable, like page
 *  addressing command, then abstract it into some functions
 *
 * Proposed way to set value by command:
 * [START] [SlaveAddr, Command_Ctrl, Command_Data, DataToSet] [STOP]
 *
 * NOTE: Status Register Read -
 *       This command is issued by setting D/C# ON LOW during a data read (See
 *       Figure 13-1 to Figure 13-2 for parallel interface waveform). It allows
 *       the MCU to monitor the internal status of the chip. No status read is
 *       provided for serial mode
 *
 * For I2C Interface Timinng charesteristics, read page 54 of SSD1306 datasheet
 *
 **/

typedef enum
{
  PAGE_ADDRESSING = 0x2,
  VERT_ADDRESSING = 0x1,
  HORZ_ADDRESSING = 0x0,
  INVALID = 0x3
} ssd1603_addr_mode_e;

typedef enum
{
  // 1. Fundemental Command Table
  E_SET_CONTRAST_CTRL = 0x0a81, // Reset == 0x7f
  E_ENTIRE_DISPLAY_RESUME = 0xa4,
  E_ENTIRE_DISPLAY_ON = 0xa5,
  E_SET_NORMAL_DISPLAY = 0xa6,
  E_SET_INVERTED_DISPLAY = 0xa7,
  E_SET_DISPLAY_OFF = 0xae,
  E_SET_DISPLAY_ON = 0xaf,

  // 2. Scrolling Command Table
  E_RIGHT_HORIZONTAL_SCROLL = 0x26, // Scroll by 1 column
  E_LEFT_HORIZONTAL_SCROLL = 0x27, // Scroll by 1 column
  E_VERT_HORIZONTAL_SCROLL = 0x29, // Scroll by 1 column
  E_HORZ_HORIZONTAL_SCROLL = 0x2a, // Scroll by 1 column (table at p.29)
  E_STOP_SCROLL = 0x2e,
  E_ACTIVATE_SCROLL = 0x2f,
  E_SET_VERT_SCROLL_AREA = 0xa3,

  // 3. Addressing Setting Command Table
  E_SET_LOWER_COL_STARTADDR_PAM = 0x00,
  E_SET_HIGHER_COL_STARTADDR_PAM = 0x10,
  E_SET_MEM_ADDR_MODE = 0x20,
  E_SET_COL_ADDR = 0x21,
  E_SET_PAGE_ADDR = 0x22,
  E_SET_PAGE_STARTADDR_PAM = 0xb0,

  // 4. HW Config (Panel resolution & layour related) Command Table
  E_SET_DISPLAY_START_LINE = 0x40, /** 0x40 - 0x7f */
  E_SET_SEGMENT_REMAP_0 = 0xa0, // Col 0 is mapped to SEG0
  E_SET_SEGMENT_REMAP_127 = 0xa0, // Col 127 is mapped to SEG0
  E_SET_MUX_RATIO = 0xa8, // Set mux ratio to N+1 MUX,
                          // [0,14]invalid, [16,63]valid
  E_SET_COM_NORMAL_SCAN = 0xc0, // COM output scan direction
  E_SET_COM_REMAPPED_SCAN = 0xc8, // COM output scan direction
  E_SET_DISPLAY_OFFSET = 0xd3, // Vertical shift by COM, [0,63]
  E_SET_COM_PINS_HWCONF = 0xda,

  // 5. Timing & Driving Scheme Setting Command Table
  E_SET_DSPL_CLK_DROF = 0xd5,
  E_SET_PRECHARGE_PERIOD = 0xd9,
  E_SET_VCOM_DESLCT_LVL = 0xdb,
  E_NO_OP_CMD = 0xe3,

} ssd1306_cmd_registers_e; // Please refer to the command table at chapter 9
                           // (p.28-p.32) for register bit field descriptions
#define ARM_M7
//#define ATMETGA32
//#define ...

#define SSD1306_I2C_SET_LOW_COL_STARTADDR_PAM(x)                               \
  (E_SET_LOWER_COL_STARTADDR_PAM + (x))
#define SSD1306_I2C_SET_HIGHER_COL_STARTADDR_PAM(x)                            \
  (E_SET_HIGHER_COL_STARTADDR_PAM + (x))
#define SSD1306_I2C_SET_PAGE_STARTADDR_PAM(x) (E_SET_PAGE_STARTADDR_PAM + (x))
#define SSD1306_I2C_SET_DISPLAY_START_LINE(x) (E_SET_DISPLAY_START_LINE + (x))

#define ssd1306_delay(x) _delay_loop_2(x)

/** Section, which defines I2C timings for SSD1306 display from datasheet */
#define SSD1306_I2C_START_STOP_DELAY 600
#define SSD1306_I2C_RISE_TIME 300
#define SSD1306_I2C_FALL_TIME 300
#define SSD1306_I2C_DATA_HOLD_TIME 300
#define SSD1306_I2C_IDLE_TIME 1300
#define SSD1306_I2C_CLOCK 2500

/** Send Start to SSD1306 display. */
typedef void (*start_fp_t)(void);

/** Send Stop condition to SSD1306 display. */
typedef void (*stop_fp_t)(void);

/** Sends byte to SSD1306 device; @param data - TX - byte */
typedef void (*send_fp_t)(unsigned char data);

/**
 * @brief Sends bytes to SSD1306 device;
 * @param buf byte buffer; @param size # of bytes;
 */
typedef void (*sendbuf_fp_t)(const unsigned char * buf,
                             unsigned short        size,
                             trigger_gpio_fp_t     trigger_gpio);

typedef struct {
  start_fp_t   start_fp;
  stop_fp_t    stop_fp;
  send_fp_t    send_fp;
  sendbuf_fp_t send_buffer_fp;
} ssd1306_intf; // ssd1306 interface

// physical layer
static void
ssd1306_phy_send_byte(unsigned char data, trigger_gpio_fp_t trigger_gpio);
static void
ssd1306_phy_send_bytes(const unsigned char * buf,
                       unsigned short        size,
                       trigger_gpio_fp_t     trigger_gpio);
static void
ssd1306_phy_start(void);
static void
ssd1306_phy_stop(void);
static void
ssd1306_phy_close();
void
ssd1306_phy_init(char scl, char sda, char sa);

// abstracted layer
ssd1306_intf *
ssd1306_create_interface();

void
ssd1306_set_address_mode(ssd1603_addr_mode_e memory_mode);
void
ssd1306_send_command_byte(ssd1306_cmd_registers_e cmd_hex);
void
ssd1306_set_command_register_byte(ssd1306_cmd_registers_e cmd_hex,
                                  unsigned char           value);
void
ssd1306_set_command_register_word(ssd1306_cmd_registers_e cmd_hex,
                                  unsigned short          value);
void
ssd1306_set_command_register_dword(ssd1306_cmd_registers_e cmd_hex,
                                   unsigned long           value);
void
ssd1306_send_data_byte(unsigned char data);
void
ssd1306_send_data_word(unsigned short data);
void
ssd1306_send_data_dword(unsigned long data);

/**
 * 128x64 MINIOLED
 * OLED Driver IC: SSD1306 (SOLOMON SYSTECH)
 * Resolution: 128 x 64
 * Visual Angle: >160°
 * Input Voltage: 3.3V ~ 6V
 * Compatible I/O Level: 3.3V, 5V
 * Mini Size: 2.7 x 2.8cm
 * Only Need 2 I/O Port to Control
 * Full Compatible , 51
 * Relevant pins: GND, VCC, SCL, SDA
 *   (Info regarding relevant pins is in the 128x32 MINIOLED description above)
 **/

/**
 * 3.2 inch TFT LCD
 * OLED Driver IC: IL9341
 * Resolution: 240 x 320
 * Visual Angle: >160°
 * Input Voltage: 3.3V
 * Compatible I/O Level: 3.3V
 * Mini Size: 9.4 x 6.1 cm
 **/

#endif // DISPLAY_DRIVER_H