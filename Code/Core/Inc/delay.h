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
* @file   delay.h
* @brief  A file declaring the delay related APIs.
*
* @author Krishna Suhagiya and Sriya Garde
* @date   November 8, 2023
* @revision 1.0
*
*/

#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>

/**
 * @brief   A function to get the current time in milliseconds.
 *
 * @param   NULL
 *
 * @return  Current time.
 */
uint32_t millis(void);

/**
 * @brief   A function to initialize the systick.
 *
 * @param   The desired frequency is hertz.
 *
 * @return  None.
 */
void systick_init_ms(uint32_t freq);

/**
 * @brief   A function to delay the system by the specified milliseconds.
 *
 * @param   The desired delay in milliseconds.
 *
 * @return  None.
 */
void delay(uint32_t ms);

#endif /* __DELAY_H */
