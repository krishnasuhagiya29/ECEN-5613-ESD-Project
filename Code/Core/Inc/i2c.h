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
* @file i2c.h
* @brief  A file declaring the I2C communication protocol related APIs for communication between OLED and STM32.
*
* @author Krishna Suhagiya
* @date   November 21, 2023
* @revision 1.0
* @reference https://blog.embeddedexpert.io/?p=613
*
*/

#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>
#include "stm32f4xx.h"
#include "delay.h"

/**
 * @brief   A function to initialize the I2C.
 *
 * @param   None.
 *
 * @return  None.
 */
void i2c_init(void);

/**
 * @brief   A function to write a single byte of data to a specified memory address of a slave device.
 *
 * @param   saddr Slave address
 *          madr  Memory address
 *          data  Data byte
 *
 * @return  None.
 */
void i2c_write_byte(char saddr, char maddr, char data);

/**
 * @brief   A function to write multiple bytes of data to a specified memory address of a slave device.
 *
 * @param   saddr Slave address
 *          madr  Memory address
 *          buffer Pointer to the buffer array
 *          length The length of the buffer
 *
 * @return  None.
 */
void i2c_write_multi(char saddr, char maddr, char *buffer, uint8_t length);

#endif	// __I2C_H

