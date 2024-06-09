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
* @file     UART.h
* @brief    A file declaring the APIs for UART.
*
* @author   Sriya Garde
* @date     November 8, 2023
* @revision 1.0
*
*/

#ifdef DEBUG
#ifndef UART_H_
#define UART_H_
#include "stm32f4xx.h"

#define RCC_GPIOA_ENR         (0b01)
#define RCC_GPIOD_ENR         (0b1 << 3)
#define RCC_TIM2_ENR          (0b01)
#define GPIOA_PORT0_INPUT     (0b11)
#define GPIOD_PORT12_OUTPUT   (0b01 << 24)
#define GPIOD_PORT15_OUTPUT   (0b01 << 30)
#define PORT12                (0B01<<12)
#define PORT15                (0B01<<15)

/**
 * @brief   A function to initialize UART2 for to receive or transmit characters and strings.
 *
 * @param   None
 *
 * @return  None.
 */
void USART2_init(void);

/**
 * @brief   A function to transmit given character input.
 *
 * @param   Character to transmit
 *
 * @return  Transmitted character.
 */
char USART2_transmit(char input);

/**
 * @brief   A function to receive given character transmitted.
 *
 * @param   NULL
 *
 * @return  Character received.
 */
char USART2_receive(void);

/**
 * @brief   A function to receive given string transmitted.
 *
 * @param   Characters received through receive function
 *
 * @return  NULL
 */
void USART2_string_transmit(char *text);

#endif /* UART_H_ */
#endif
