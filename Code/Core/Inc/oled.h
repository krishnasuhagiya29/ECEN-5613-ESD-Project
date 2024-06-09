/*****************************************************************************
* Copyright (C) 2023 by Krishna Suhagiya
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Krishna Suhagiya and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file   oled.h
* @brief  A file declaring the OLED SSD 1106 display related APIs.
*
* @author Krishna Suhagiya
* @date   November 21, 2023
* @reference https://blog.embeddedexpert.io/?p=613
* @revision 1.0
*
*/

#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"
#include <stdlib.h>
#include <string.h>
#include "fonts.h"
#include "i2c.h"

/**
 * This SSD1106 LCD uses I2C for communication
 *
 * Default pinout
 *
 SSD1106    |STM32F411RE  |DESCRIPTION

 VCC        |3.3V         |
 GND        |GND          |
 SCL        |PB8          |Serial clock line
 SDA        |PB9          |Serial data line
 */

/* I2C address */
#ifndef SSD1106_I2C_ADDR
#define SSD1106_I2C_ADDR         0x3C
#endif

/* SSD1106 settings */
/* SSD1106 width in pixels */
#ifndef SSD1106_WIDTH
#define SSD1106_WIDTH            128
#endif
/* SSD1106 LCD height in pixels */
#ifndef SSD1106_HEIGHT
#define SSD1106_HEIGHT           64
#endif

typedef enum {
	SSD1106_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	SSD1106_COLOR_WHITE = 0x01 /*!< Pixel is set. Color depends on LCD */
} SSD1106_COLOR_t;

/**
 * @brief   A function to initialize the OLED.
 *
 * @param   None
 *
 * @return  Initialization status.
 */
uint8_t SSD1106_init(void);

/**
 * @brief   A function to update the OLED display with the configuration.
 *
 * @param   None
 *
 * @return  None.
 */
void SSD1106_update_screen(void);

/**
 * @brief   A function to fill the OLED display with the specified color.
 *
 * @param   Enumerated value of the color
 *
 * @return  None.
 */
void SSD1106_fill(SSD1106_COLOR_t Color);

/**
 * @brief   A function to draw a pixel at the specified coordinates(x,y) with a specified color.
 *
 * @param   x     Value of X-coordinate
 *          y     Value of Y-coordinate
 *          color Enumerated value of the color
 *
 * @return  None.
 */
void SSD1106_draw_pixel(uint16_t x, uint16_t y, SSD1106_COLOR_t color);

/**
 * @brief   A function to move the cursor to the specified coordinates(x,y).
 *
 * @param   x     Value of X-coordinate
 *          y     Value of Y-coordinate
 *
 * @return  None.
 */
void SSD1106_gotoXY(uint16_t x, uint16_t y);

/**
 * @brief   A function to display a character on the OLED display.
 *
 * @param   ch    Character to display
 * 			Font  Pointer to the font size structure
 *          color Enumerated value of the color
 *
 * @return  The character displayed.
 */
char SSD1106_putc(char ch, FontDef_t *Font, SSD1106_COLOR_t color);

/**
 * @brief   A function to display a string on the OLED display.
 *
 * @param   ch    String to display
 * 			Font  Pointer to the font size structure
 *          color Enumerated value of the color
 *
 * @return  Pointer to the string displayed.
 */
char SSD1106_puts(char *str, FontDef_t *Font, SSD1106_COLOR_t color);

/**
 * @brief   A function to clear the entire OLED display.
 *
 * @param   None
 *
 * @return  None.
 */
void SSD1106_clear_screen(void);

/**
 * @brief   A function to clear one line of OLED display.
 *
 * @param   None
 *
 * @return  None.
 */
void SSD1106_clear_line(void);

/**
 * @brief   A function to settle I2C initialization for OLED to STM32 communication.
 *
 * @param   None
 *
 * @return  None.
 */
void SSD1106_i2c_init(void);

/**
 * @brief   A wrapper function to write multiple bytes of data to a specified memory address of a slave device.
 *
 * @param   saddr Slave address
 *          madr  Memory address
 *          buffer Pointer to the buffer array
 *          length The length of the buffer
 *
 * @return  None.
 */
void SSD1106_i2c_write_multi(uint8_t address, uint8_t reg, char *data,
		uint16_t count);

#endif	/* __OLED_H */
