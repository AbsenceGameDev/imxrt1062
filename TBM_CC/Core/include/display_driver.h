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
 *       disconnected, the acknowledgement signal will be ignored in the I2C-bus.
 *
 * Regarding SCL and SDA for data comms -
 *  Read chapter 8.1.5 (p.19-21) MINIOLED_SSD1206.pdf
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