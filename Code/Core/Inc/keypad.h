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
* @file      keypad.h
* @brief     A file declaring the APIs for 4x3 matrix keypad.
*
* @author    Sriya Garde
* @date      November 18, 2023
* @revision  1.0
* @reference https://www.youtube.com/watch?v=R5sv1hbONrk
*            https://learn.parallax.com/tutorials/language/propeller-c/propeller-c-simple-devices/read-4x4-matrix-keypad
*
*/

#ifndef __KEYPAD_H
#define __KEYPAD_H
#include "stm32f4xx.h"

/**
 * @brief   A function to initialize the keypad.
 *
 * @param   None.
 *
 * @return  None.
 */
void init_keypad(void);

/**
 * @brief   A function to check the correctness of the entered keys for password.
 *
 * @param   None.
 *
 * @return  Pointer to the key inputs.
 */
char* check_key(void);

#endif /* __KEYPAD_H */
