/*****************************************************************************
* Copyright (C) 2023 by Sriya Garde
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Sriya Garde and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file      keypad.c
* @brief     A file defining the APIs for 4x3 matrix keypad.
*
* @author    Sriya Garde
* @date      November 18, 2023
* @reference https://www.youtube.com/watch?v=R5sv1hbONrk
*            https://learn.parallax.com/tutorials/language/propeller-c/propeller-c-simple-devices/read-4x4-matrix-keypad
* @revision  1.0
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "UART.h"
#include "keypad.h"
#include "delay.h"

volatile uint8_t buttonState[3][3] = { 0 };
uint32_t volatile *RCC_Base_Addr = (uint32_t*) 0x40023830;
uint32_t volatile *GPIOModer = (uint32_t*) 0x40020800;

uint32_t volatile *GPIOOutput = (uint32_t*) 0x40020814;
uint32_t volatile *GPIOIntput = (uint32_t*) 0x40020810;
uint32_t volatile *GPIOPullup = (uint32_t*) 0x4002080C;
uint32_t volatile *GPIOOspeed = (uint32_t*) 0x40020808;

char key_data[50] = { 0 };

void init_keypad(void) {

	//Base address RCC 0x4002 3800 + Address offset of RCC AHB1 = 0x30
	//Base address GPIOE 0x4002 1000 + Address offset of GPIO Moder = 0x00
	//GPIO Port o/p data register = 0x14

	*RCC_Base_Addr |= 1 << 2; //Setting the 4th bit of AHB1ENR for giving clock to port C

	*GPIOModer &= ~(0xFF << 8); //Cols as Input , so 00 to E4,E5,E6 as input

	//*GPIOModer &= ~(0xFF<<0); //Clearing the bits before setting them
	*GPIOModer |= (0x55 << 0); //01010101 to set Output for E0,E1,E2,E3 pins

	*GPIOOspeed &= ~(0xFF << 0);

	*GPIOPullup &= ~(0xFF << 8);
	*GPIOPullup |= (0x15 << 8);

}

char* check_key(void) {
//*****************************************ROW 1*********************************************************
	char ch = '\0';
	bool button_pressed = false;
	int i = 0;
	memset(key_data, 0, 50);

	while (1) {
		*GPIOOutput |= (0xFF << 0);	// Set the value

		*GPIOOutput &= ~(1 << 0);	// 0th row

		if (!(*GPIOIntput & (1 << 4)) && !buttonState[0][0]) {	// Check GPIO state
			button_pressed = true;
			ch = '1';
			buttonState[0][0] = 1;
			delay(50); // Debounce delay

		} else if (*GPIOIntput & (1 << 4)) {
			buttonState[0][0] = 0;
		}

		if (!(*GPIOIntput & (1 << 5)) && !buttonState[0][1]) {	// Check GPIO state
			button_pressed = true;
			ch = '2';
			buttonState[0][1] = 1;
			delay(50); // Debounce delay
		} else if (*GPIOIntput & (1 << 5)) {
			buttonState[0][1] = 0;
		}

		if (!(*GPIOIntput & (1 << 6)) && !buttonState[0][2]) {	// Check GPIO state
			button_pressed = true;
			ch = '3';
			buttonState[0][2] = 1;
			delay(50); // Debounce delay
		} else if (*GPIOIntput & (1 << 6)) {
			buttonState[0][2] = 0;
		}

//*****************************************ROW 2*********************************************************

		*GPIOOutput |= (0x0F << 0);

		*GPIOOutput &= ~(1 << 1);	// 1st row

		if (!(*GPIOIntput & (1 << 4)) && !buttonState[1][0]) {	// Check GPIO state
			button_pressed = true;
			ch = '4';
			buttonState[1][0] = 1;
			delay(50); // Debounce delay

		} else if (*GPIOIntput & (1 << 4)) {
			buttonState[1][0] = 0;
		}

		if (!(*GPIOIntput & (1 << 5)) && !buttonState[1][1]) {	// Check GPIO state
			button_pressed = true;
			ch = '5';
			buttonState[1][1] = 1;
			delay(50); // Debounce delay
		} else if (*GPIOIntput & (1 << 5)) {
			buttonState[1][1] = 0;
		}

		if (!(*GPIOIntput & (1 << 6)) && !buttonState[1][2]) {	// Check GPIO state
			button_pressed = true;
			ch = '6';
			buttonState[1][2] = 1;
			delay(50); // Debounce delay
		} else if (*GPIOIntput & (1 << 6)) {
			buttonState[1][2] = 0;
		}

//*****************************************ROW 3*********************************************************
		*GPIOOutput |= (0x0F << 0);

		*GPIOOutput &= ~(1 << 2);	// 2nd row

		if (!(*GPIOIntput & (1 << 4)) && !buttonState[2][0]) {	// Check GPIO state
			button_pressed = true;
			ch = '7';
			buttonState[2][0] = 1;
			delay(50); // Debounce delay

		} else if (*GPIOIntput & (1 << 4)) {
			buttonState[2][0] = 0;
		}

		if (!(*GPIOIntput & (1 << 5)) && !buttonState[2][1]) {	// Check GPIO state
			button_pressed = true;
			ch = '8';
			buttonState[2][1] = 1;
			delay(50); // Debounce delay
		} else if (*GPIOIntput & (1 << 5)) {
			buttonState[2][1] = 0;
		}

		if (!(*GPIOIntput & (1 << 6)) && !buttonState[2][2]) {	// Check GPIO state
			button_pressed = true;
			ch = '9';
			buttonState[2][2] = 1;
			delay(50); // Debounce delay
		} else if (*GPIOIntput & (1 << 6)) {
			buttonState[2][2] = 0;
		}

//*****************************************ROW 4*********************************************************

		*GPIOOutput |= (0x0F << 0);

		*GPIOOutput &= ~(1 << 3);	// 4th row

		if (!(*GPIOIntput & (1 << 4)) && !buttonState[3][0]) {	// Check GPIO state
			//USART2_StringTransmit("* \r\n");
			button_pressed = true;
			ch = '*';
			buttonState[3][0] = 1;
			delay(50); // Debounce delay

		} else if (*GPIOIntput & (1 << 4)) {
			buttonState[3][0] = 0;
		}

		if (!(*GPIOIntput & (1 << 5)) && !buttonState[3][1]) {	// Check GPIO state
			//USART2_StringTransmit("0 \r\n");
			button_pressed = true;
			ch = '0';
			buttonState[3][1] = 1;
			delay(50); // Debounce delay
		} else if (*GPIOIntput & (1 << 5)) {
			buttonState[3][1] = 0;
		}

		if (!(*GPIOIntput & (1 << 6)) && !buttonState[3][2]) {	// Check GPIO state
			//USART2_StringTransmit("# \r\n");
			button_pressed = true;
			ch = '#';
			buttonState[3][2] = 1;
			delay(50); // Debounce delay
		} else if (*GPIOIntput & (1 << 6)) {
			buttonState[3][2] = 0;
		}
		if (ch == '#')
			break;												// Break if delimiter detected
		if ((button_pressed == true) && (!((!(ch == '*')) || (ch == '#')))) {
#ifdef DEBUG
			USART2_string_transmit(
					"Invalid input. Please enter digits only.\r\n");
#endif
		} else if ((button_pressed == true) && ((ch >= '0') && (ch <= '9'))) {
			key_data[i] = ch;
			i++;
			ch = '\0';
		}
	}
	return key_data;

}

