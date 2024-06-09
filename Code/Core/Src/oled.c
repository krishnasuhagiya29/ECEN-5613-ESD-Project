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
* @file      oled.c
* @brief     A file defining the OLED SSD 1106 display related APIs.
*
* @author    Krishna Suhagiya
* @date      November 21, 2023
* @reference https://blog.embeddedexpert.io/?p=613
* @revision 1.0
*
*/

#include "oled.h"

//#define SSD1106_I2C_ADDR 0x3C
/* Write command */
#define SSD1106_WRITECOMMAND(command)      i2c_write_byte(SSD1106_I2C_ADDR, 0x00, (command))

/* SSD1106 data buffer */
static char SSD1106_Buffer[SSD1106_WIDTH * SSD1106_HEIGHT / 8];

/* Private SSD1106 structure */
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Initialized;
} SSD1106_t;

/* Private variable */
static SSD1106_t SSD1106;

#define SSD1106_DEACTIVATE_SCROLL                    0x2E // Stop scroll

uint8_t SSD1106_init(void) {

	/* Init I2C */
	i2c_init();
	SSD1106_i2c_init();

	/* A little delay */
	uint32_t p = 2500;
	while (p > 0)
		p--;

	/* Init LCD */
	SSD1106_WRITECOMMAND(0xAE); //display off
	SSD1106_WRITECOMMAND(0x20); //Set Memory Addressing Mode
	SSD1106_WRITECOMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	SSD1106_WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	SSD1106_WRITECOMMAND(0xC8); //Set COM Output Scan Direction
	SSD1106_WRITECOMMAND(0x00); //set low column address
	SSD1106_WRITECOMMAND(0x10); //set high column address
	SSD1106_WRITECOMMAND(0x40); //set start line address
	SSD1106_WRITECOMMAND(0x81); //set contrast control register
	SSD1106_WRITECOMMAND(0xFF);
	SSD1106_WRITECOMMAND(0xA1); //set segment re-map 0 to 127
	SSD1106_WRITECOMMAND(0xA6); //set normal display
	SSD1106_WRITECOMMAND(0xA8); //set multiplex ratio(1 to 64)
	SSD1106_WRITECOMMAND(0x3F); //
	SSD1106_WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	SSD1106_WRITECOMMAND(0xD3); //set display offset
	SSD1106_WRITECOMMAND(0x00); //not offset
	SSD1106_WRITECOMMAND(0xD5); //set display clock divide ratio/oscillator frequency
	SSD1106_WRITECOMMAND(0xF0); //set divide ratio
	SSD1106_WRITECOMMAND(0xD9); //set pre-charge period
	SSD1106_WRITECOMMAND(0x22); //
	SSD1106_WRITECOMMAND(0xDA); //set com pins hardware configuration
	SSD1106_WRITECOMMAND(0x12);
	SSD1106_WRITECOMMAND(0xDB); //set vcomh
	SSD1106_WRITECOMMAND(0x20); //0x20,0.77xVcc
	SSD1106_WRITECOMMAND(0x8D); //set DC-DC enable
	SSD1106_WRITECOMMAND(0x14); //
	SSD1106_WRITECOMMAND(0xAF); //turn on SSD1106 panel

	SSD1106_WRITECOMMAND(SSD1106_DEACTIVATE_SCROLL);

	SSD1106_fill(SSD1106_COLOR_BLACK);	// Clear screen

	SSD1106_update_screen();	// Update screen

	/* Set default values */
	SSD1106.CurrentX = 0;
	SSD1106.CurrentY = 0;

	SSD1106.Initialized = 1;	// Initialized OK

	return SSD1106.Initialized;
}

void SSD1106_update_screen(void) {
	uint8_t m;

	for (m = 0; m < 8; m++) {
		SSD1106_WRITECOMMAND(0xB0 + m);
		SSD1106_WRITECOMMAND(0x00);
		SSD1106_WRITECOMMAND(0x10);

		/* Write multi data */
		SSD1106_i2c_write_multi(SSD1106_I2C_ADDR, 0x40,
				&SSD1106_Buffer[SSD1106_WIDTH * m], SSD1106_WIDTH);
	}
}

void SSD1106_fill(SSD1106_COLOR_t color) {
	/* Set memory */
	memset(SSD1106_Buffer, (color == SSD1106_COLOR_BLACK) ? 0x00 : 0xFF,
			sizeof(SSD1106_Buffer));
}

void SSD1106_draw_pixel(uint16_t x, uint16_t y, SSD1106_COLOR_t color) {
	if (x >= SSD1106_WIDTH || y >= SSD1106_HEIGHT) {
		return;
	}

	/* Set color */
	if (color == SSD1106_COLOR_WHITE) {
		SSD1106_Buffer[x + (y / 8) * SSD1106_WIDTH] |= 1 << (y % 8);
	} else {
		SSD1106_Buffer[x + (y / 8) * SSD1106_WIDTH] &= ~(1 << (y % 8));
	}
}

void SSD1106_gotoXY(uint16_t x, uint16_t y) {
	/* Set write pointers */
	SSD1106.CurrentX = x;
	SSD1106.CurrentY = y;
}

char SSD1106_putc(char ch, FontDef_t *Font, SSD1106_COLOR_t color) {
	uint32_t i, b, j;

	/* Check available space in LCD */
	if (
	SSD1106_WIDTH <= (SSD1106.CurrentX + Font->FontWidth) ||
	SSD1106_HEIGHT <= (SSD1106.CurrentY + Font->FontHeight)) {
		return 0;
	}

	/* Go through font */
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				SSD1106_draw_pixel(SSD1106.CurrentX + j, (SSD1106.CurrentY + i),
						(SSD1106_COLOR_t) color);
			} else {
				SSD1106_draw_pixel(SSD1106.CurrentX + j, (SSD1106.CurrentY + i),
						(SSD1106_COLOR_t) !color);
			}
		}
	}

	/* Increase pointer */
	SSD1106.CurrentX += Font->FontWidth;

	/* Return character written */
	return ch;
}

char SSD1106_puts(char *str, FontDef_t *Font, SSD1106_COLOR_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (SSD1106_putc(*str, Font, color) != *str) {
			/* Return error */
			return *str;
		}

		str++;
	}

	return *str;
}

void SSD1106_clear_screen(void) {
	SSD1106_fill(0);				// Fill the buffer with zeros
	SSD1106_update_screen();		// Update the screen with the filled buffer
}

void SSD1106_clear_line(void) {
	SSD1106_puts("                  ", &Font_7x10, 1);	// Clear the whole line from the current cursor position
}

void SSD1106_i2c_init() {

	uint32_t p = 250000;			// Wait for I2C initialization
	while (p > 0)
		p--;

}

void SSD1106_i2c_write_multi(uint8_t address, uint8_t reg, char *data,
		uint16_t count) {
	i2c_write_multi(address, reg, data, count);		// I2C write multi-registers
}
