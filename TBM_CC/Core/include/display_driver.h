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
 * (8.1.5.2 p.20) Steps to set-up write mode I2C
 *  1. The master device initiates the data communication by a start condition.
 *     The definition of the start condition is shown in Figure 8-8.
 *     The start condition is established by pulling the SDA from HIGH to LOW
 *     while the SCL stays HIGH.
 *  2. The slave address is following the start condition for recognition use.
 *     For the SSD1306, the slave address is either “b0111100” or “b0111101” by
 *     changing the SA0 to LOW or HIGH (D/C pin acts as SA0).
 *  3. The write mode is established by setting the R/W# bit to logic “0”.
 *  4. An acknowledgement signal will be generated after receiving one byte of
 *     data, including the slave address and the R/W# bit. Please refer to the
 *     Figure 8-9 for the graphical representation of the acknowledge signal.
 *     The acknowledge bit is defined as the SDA line is pulled down during the
 *     HIGH period of the acknowledgement related clock pulse.
 *  5. After the transmission of the slave address, either the control byte or
 *     the data byte may be sent across the SDA. A control byte mainly consists
 *     of Co and D/C# bits following by six “0” ‘s.
 *        a.If the Co bit is set as logic “0”, the transmission of the following
 *          information will contain data bytes only.
 *        b.The D/C# bit determines the next data byte is acted as a command or
 *          a data. If the D/C# bit is set to logic “0”, it defines the
 *          following data byte as a command. If the D/C# bit is set to logic
 *          “1”, it defines the following data byte as a data which will be
 *          stored at the GDDRAM. The GDDRAM column address pointer will be
 *          increased by one automatically after each data write.
 *  6. Acknowledge bit will be generated after receiving each control byte or
 *     data byte.
 *  7. The write mode will be finished when a stop condition is applied. The
 *     stop condition is also defined in Figure 8-8. The stop condition is
 *     established by pulling the “SDA in” from LOW to HIGH while the “SCL”
 *     stays HIGH.
 *
 * NOTE:
 *      Please be noted that the transmission of the data bit has some
 *      limitations.
 *      1. The data bit, which is transmitted during each SCL pulse, must
 *         keep at a stable state within the “HIGH” period of the clock pulse.
 *         Please refer to the  Figure 8-10 for graphical representations.
 *         Except in start or stop conditions, the data line can be switched
 *         only when the SCL is LOW.
 *      2. Both the data line (SDA) and the clock line (SCL) should be pulled up
 *         by external resistors.
 *
 * NOTE:
 *      Solomon Systech Apr 2008   P 22/59 Rev 1.1 SSD1306  8.2Command Decoder
 *      This module determines whether the input data is interpreted as data or
 *      command. Data is interpreted based upon the input of the D/C# pin.
 *
 *      If D/C# pin is HIGH, D[7:0] is interpreted as display data written to
 *      Graphic Display Data RAM (GDDRAM). If it is LOW, the input at D[7:0] is
 *      interpreted as a command. Then data input will be decoded and written to
 *      the corresponding command register.
 *
 * NOTE:
 *      The frame frequency of display is determined by the following formula:
 *      F{frame} = F{osc} / (D * K * No.of Mux)
 *      where:
 *      • D stands for clock divide ratio. It is set by command D5h A[3:0].
 *        The divide ratio has the range from 1 to 16.
 *      • K is the number of display clocks per row.  The value is derived by
 *        K = Phase 1 period + Phase 2 period + BANK0 pulse width
 *          =  2 + 2 + 50 = 54 at power on reset
 *          (Please refer to Section 8.6 “Segment Drivers / Common Drivers” for
 *           the details of the “Phase”)
 *      • Number of multiplex ratio is set by command A8h. The power on reset
 *        value is 63 (i.e. 64MUX).
 *      • FOSC is the oscillator frequency. It can be changed by command
 *        D5h A[7:4]. The higher the register setting results in higher freq.
 *
 * SUMMARY:
 *  First I need to power on the lcd with the above mentioned power-onn
 *  sequence. Then after it is powered on I send a boot up command, from there I
 *  need to set som initializing commands. When data needs to be sent to the
 *  screen I must also deliver a signal through SCL input to triger the clock.
 *  The clock needs to be signalled before data from SDA can be read.
 *
 * Now the question remains how the data is actually being read, and if I can
 *  gain direct access to the screen through the SDA pin somehow
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