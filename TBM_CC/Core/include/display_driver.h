#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

/**
 * @brief 128x32 MINIOLED
 * OLED Driver IC: SSD1306 (SOLOMON SYSTECH)
 * Resolution: 128 x 32
 * Visual Angle: >160°
 * Input Voltage: 3.3V ~ 5V
 * Compatible I/O Level: 3.3V, 5V
 * Only Need 2 I/O Port to Control
 * Relevant pins: GND, VC0, SCL, SDA
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
 * Regarding SCL and SDA for data comms -
 *  Read chapter 8.1.5 (p.19-21) MINIOLED_SSD1206.pdf
 *  8.1.5 MCU I2C Interface -
 *
 *
 * Regarding whether input is data or commands -
 *  Read chapter 8.2 (p.22) MINIOLED_SSD1206.pdf
 *  Read chapter 9. (p.28-33) MINIOLED_SSD1206.pdf (Commands)
 *  Read chapter 10. (p.34-46) MINIOLED_SSD1206.pdf (Commands)
 *
 * Regarding Powering On/Off -
 *  Read chapter 8.9 (p.27) MINIOLED_SSD1206.pdf
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
 * Summary:
 *  When data needs to be sent to the screen I must also deliver a signal
 *  through SCL input to triger teh clock. The clock needs to be signalled
 *  before data from SDA can be read.
 *
 * Now the question remains how the data is actually being read, and if I can
 *gain direct access to the screen through the SDA pin somehow
 *
 **/

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
 * Relevant pins: GND, VC0, SCL, SDA
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