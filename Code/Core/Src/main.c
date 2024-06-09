/*****************************************************************************
* Copyright (C) 2023 by Krishna Suhagiya and Sriya Garde
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Krishna Suhagiya, Sriya Garde and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file   main.c
* @brief  A main file demonstrating the security access control system.
*
* @author Krishna Suhagiya and Sriya Garde
* @date   November 8, 2023
*
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "UART.h"
#include "delay.h"
#include "rfid.h"
#include "voice.h"
#include "beeper.h"
#include "oled.h"
#include "keypad.h"
#include "security_system_interface.h"

#define SIXTEEN_MHZ	16000000

int main(void) {
	systick_init_ms(SIXTEEN_MHZ);	// Initialize system clock
	beeper_init();					// Initialize buzzer (beeper)
	RC522_init();					// Initialize RFID reader module
	SSD1106_init();					// Initialize OLED display
	SSD1106_gotoXY(0, 0);			// Set the cursor to (0,0) location on the OLED
	init_keypad();					// Initialize keypad
#ifdef DEBUG
	USART2_init();
	USART2_string_transmit("Please tap card \r\n");
#endif
	SSD1106_clear_screen();			// Clear the OLED screen
	SSD1106_puts("Please tap card", &Font_7x10, 1);	// Set the default message to be displayed on OLED
	SSD1106_gotoXY(0, 10);			// Set the cursor to (0,10) location
	SSD1106_clear_line();			// Clear the line
	SSD1106_gotoXY(0, 20);			// Set the cursor to (0,20) location
	SSD1106_clear_line();			// Clear the line
	SSD1106_update_screen(); 		// Display the content set above on OLED

	while (1) {
		check_access();				// Check the card access on every tap
	}
}
