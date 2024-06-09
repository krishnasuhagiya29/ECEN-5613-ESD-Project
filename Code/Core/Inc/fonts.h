/*****************************************************************************
* Copyright (C) 2023 by Krishna Suhagiya
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Krishna Suhagiya and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file beeper.h
* @brief  A file declaring the fonts for the OLED.
*
* @author Krishna Suhagiya
* @date   November 21, 2023
* @revision 1.0
* @reference https://blog.embeddedexpert.io/?p=613
*
*/

#ifndef __FONTS_H
#define __FONTS_H

#include "stm32f4xx.h"
#include <stdint.h>
#include <string.h>

/**
 * @brief  Font structure used on my LCD libraries
 */
typedef struct {
	uint8_t FontWidth; /*!< Font width in pixels */
	uint8_t FontHeight; /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} FontDef_t;

/** 
 * @brief  String length and height 
 */
typedef struct {
	uint16_t Length; /*!< String length in units of pixels */
	uint16_t Height; /*!< String height in units of pixels */
} FONTS_SIZE_t;

/**
 * @brief  7 x 10 pixels font size structure 
 */
extern FontDef_t Font_7x10;

#endif	// __FONTS_H
