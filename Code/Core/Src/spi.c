/*****************************************************************************
* Copyright (C) 2023 by Krishna Suhagiya and Sriya Garde
*
* Redistribution, modification, or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Krishna Suhagiya, Sriya Garde, and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file   	spi.c
* @brief  	A file defining the SPI communication protocol related APIs for communication between RC522 and STM32.
*
* @author 	Krishna Suhagiya and Sriya Garde
* @date   	November 8, 2023
* @revision 1.0
*/

#include <stdio.h>
#include <stdint.h>
#include "spi.h"
#include "stm32f4xx.h"
#include "delay.h"

#define AF5 0x05

// Function to initialize the SPI communication
void spi_init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable clock for GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable clock for SPI1

	// Set alternate function mode for GPIO pins 5, 6, 7
	GPIOA->MODER |= GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
	GPIOA->MODER &= ~(GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0);

	// Set pin 5, 6, 7 (SCK, MISO, MOSI) to high-speed mode
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;

	// Sets the alternate function to the pins to be used for SPI Communication
	GPIOA->AFR[0] |= (AF5 << 20) | (AF5 << 24) | (AF5 << 28);

	// Clears the control register 2 of SPI1
	SPI1->CR2 = 0;

	// Set SPI1 as Master, Set Baud Rate, Enable s/w Slave Management, Enable SPI1
	SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_MSTR | SPI_CR1_BR_2 | SPI_CR1_SSI | SPI_CR1_SPE;
}

// Function to transmit data over SPI
int8_t spi_transmit(uint8_t *data, uint32_t size) {
	uint8_t i = 0;
	uint32_t start = millis();

	// Used to check the value in the data register before transmission & clear it
	if (SPI1->DR) {}

	// Used to check the status register & ensure it is ready for SPI transmit operation
	if (SPI1->SR) {}

	while (i < size) {
		while (!((SPI1->SR) & SPI_SR_TXE)) {
			if (millis() - start > 1000) { // Wait for transmit buffer to be empty
				printf("TXE timed out\r\n");
				return -1;
			}
		}

		SPI1->DR = data[i];  // Transmit data byte by byte

		while (!(SPI1->SR & SPI_SR_BSY)) {
			if (millis() - start > 1000) {
				printf("BSY timed out\r\n");
				return -1;
			}
		}

		i++;
	}

	// Wait for Transmit buffer to be empty
	while (!((SPI1->SR) & SPI_SR_TXE)) {
		if (millis() - start > 1000) {
			printf("TXE2 timed out\r\n");
			return -1;
		}
	}

	// Wait for transmit
	while ((SPI1->SR) & SPI_SR_BSY) {
		if (millis() - start > 1000) {
			printf("BSY2 timed out\r\n");
			return -1;
		}
	}

	// Used to check the value in data register after transmission & clear it
	if (SPI1->DR) {}

	// Used to check the status register & ensure it is done with SPI receive operation
	if (SPI1->SR) {}

	return 0;
}

// Function to receive data over SPI
int8_t spi_receive(uint8_t *data, uint32_t size) {
	while (size) {
		uint32_t start = millis();
		SPI1->DR = 0;

		while (!(SPI1->SR & SPI_SR_RXNE)) {
			if (millis() - start > 200) {
				return -1;
			}
		}

		*data++ = (SPI1->DR);
		size--;
	}

	return 0;
}
