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
* @file   spi.h
* @brief  A file declaring the SPI communication protocol related APIs for communication between RC522 and STM32.
*
* @author Krishna Suhagiya and Sriya Garde
* @date   November 8, 2023
* @revision 1.0
*
*/

#ifndef __SPI_H
#define __SPI_H

#include <stdint.h>

/**
 * @brief   A function to initialize the SPI.
 *
 * @param   None
 *
 * @return  None.
 */
void spi_init(void);

/**
 * @brief   A function to transmit data over the SPI (Serial Peripheral Interface) bus.
 *
 * @param   data Pointer to the data to transmit
 *          size Size of the data to transmit
 *
 * @return  Transmission status.
 */
int8_t spi_transmit(uint8_t *data, uint32_t size);

/**
 * @brief   A function to receive data over the SPI (Serial Peripheral Interface) bus.
 *
 * @param   data Pointer to the data to receive
 *          size Size of the data to receive
 *
 * @return  Reception status.
 */
int8_t spi_receive(uint8_t *data, uint32_t size);

#endif /* __SPI_H */
