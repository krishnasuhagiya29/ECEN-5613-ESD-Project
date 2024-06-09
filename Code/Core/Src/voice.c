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
* @file     voice.c
* @brief    A file defining the APIs for voice recorder and speaker module.
*
* @author   Sriya Garde
* @date     November 25, 2023
* @revision 1.0
*
*/

#include "stm32f4xx.h"
#include "delay.h"
#include "UART.h"

void voice_init() {
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);	// Enable the AHB clock all GPIO port B

}

void voice_check() {
	GPIOD->BSRR |= GPIO_BSRR_BS1;				// Turn ON the Voice Module

	delay(10);

	GPIOD->BSRR |= GPIO_BSRR_BR1;				// Turn OFF the Voice Module
}
