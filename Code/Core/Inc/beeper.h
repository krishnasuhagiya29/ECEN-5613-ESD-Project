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
* @file beeper.h
* @brief  A file declaring the active buzzer supporting APIs.
*
* @author Sriya Garde
* @date   November 28, 2023
* @revision 1.0
*
*/

#ifndef __BEEPER_H
#define __BEEPER_H
#include "stm32f4xx.h"

/**
 * @brief   A function to initialize beeper.
 *
 * @param   NULL
 *
 * @return  NULL
 */
void beeper_init(void);

/**
 * @brief   A function to play the buzzer sound once.
 *
 * @param   NULL
 *
 * @return  NULL
 */
void beeper_enable(void);

#endif	// __BEEPER_H
