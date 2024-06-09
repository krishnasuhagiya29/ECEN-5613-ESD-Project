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
* @file   delay.c
* @brief  A file defining the delay related APIs.
*
* @author Krishna Suhagiya and Sriya Garde
* @date   November 8, 2023
* @revision 1.0
*
*/

#include "stm32f4xx.h"                  // Device header
#include "delay.h"

volatile uint32_t ms, rms;

void systick_init_ms(uint32_t freq) {
	__disable_irq();
	SysTick->LOAD = (freq / 1000) - 1;
	SysTick->VAL = 0;
	SysTick->CTRL = 7; //0b00000111;
	__enable_irq();
}

uint32_t millis(void) {
	__disable_irq();
	rms = ms; //store current ms in rms
	__enable_irq();
	return rms;
}

void SysTick_Handler(void) {
	ms++;	// Increment the milliseconds counter
}

void delay(uint32_t ms) {
	uint32_t start = millis();

	do {
		;
	} while (millis() - start < ms);

}
