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
* @file   beeper.c
* @brief  A file defining the active buzzer supporting APIs.
*
* @author Sriya Garde
* @date   November 28, 2023
* @revision 1.0
*
*/

#include "beeper.h"
#include "delay.h"

void beeper_init(void) {
	/* Enable the AHB clock for GPIO port D */
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);

	/* set Port D as output */
	GPIOD->MODER = 0X14;

}

void beeper_enable(void) {
	/* Turn ON the Buzzer */
	GPIOD->BSRR |= GPIO_BSRR_BS2;

	delay(50);

	/* Turn OFF the Buzzer */
	GPIOD->BSRR |= GPIO_BSRR_BR2;
}
