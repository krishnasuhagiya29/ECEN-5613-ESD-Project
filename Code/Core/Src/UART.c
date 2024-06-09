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
* @file     UART.c
* @brief    A file defining the APIs for UART.
*
* @author   Sriya Garde
* @date     November 8, 2023
* @revision 1.0
*
*/

#ifdef DEBUG

#include "UART.h"

void USART2_init(void) {
	RCC->APB1ENR |= (1 << 17);
	RCC->AHB1ENR |= (1 << 0);

	GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;

	GPIOA->AFR[0] |= (7 << 8) | (7 << 12);

	USART2->CR1 &= ~USART_CR1_UE;
	USART2->BRR = 0X0683;  				//9600 Baud rate 16 MHz

	USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

}

char USART2_transmit(char input) {
	while (!(USART2->SR & USART_SR_TXE))
		;								// Wait for transmit
	USART2->DR = input;
	return input;
}

char USART2_receive(void) {
	while (!(USART2->SR & USART_SR_RXNE))
		;								// Wait for receive
	return (char) USART2->DR;

}

void USART2_string_transmit(char *text) {
	while (*text)
		USART2_transmit(*text++);		// Transmit character by character
}
#endif
