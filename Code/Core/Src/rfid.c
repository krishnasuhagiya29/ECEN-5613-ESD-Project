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
* @file   	rfid.c
* @brief  	A file defining the RFID (RC522) related APIs.
*
* @author 	Krishna Suhagiya and Sriya Garde
* @date   	November 8, 2023
* @reference https://github.com/Hamid-R-Tanhaei/RFID-MIFARE-RC522-ARM-STM32/
* @revision 1.0
*/

#include "stdio.h"
#include "RFID.h"
#include "SPI.h"
#include "stdbool.h"
#include "stm32f4xx.h"
#include "delay.h"

/*
 * STM32 ->RFID
 * SPI  -> SPI
 * PA8  ->RST
 * PB0  ->CS
 * */
 
// Function to initialize the RC522 RFID reader
void RC522_init(void) {
	// Initializing SPI communication between RFID reader and STM32
	spi_init();
	GPIOA->MODER |= GPIO_MODER_MODE8_0;
	GPIOA->MODER &= ~GPIO_MODER_MODE8_1;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	GPIOB->MODER |= GPIO_MODER_MODE0_0;

	GPIOA->BSRR = GPIO_BSRR_BR8;
	delay(50);
	GPIOA->BSRR = GPIO_BSRR_BS8;
	delay(50);
	RC522_reset();

	RC522_reg_write8(MFRC522_REG_T_MODE, 0x80); // Timer starts automatically at the end of the transmission
	RC522_reg_write8(MFRC522_REG_T_PRESCALER, 0xA9); // The lower TPrescaler value
	RC522_reg_write8(MFRC522_REG_T_RELOAD_L, 0xE8); // Lower 8 bits of the 16-bit timer reload value
	RC522_reg_write8(MFRC522_REG_T_RELOAD_H, 0x03); // Higher 8 bits of the 16-bit timer reload value

	RC522_reg_write8(MFRC522_REG_TX_AUTO, 0x40);
	RC522_reg_write8(MFRC522_REG_MODE, 0x3D);

	RC522_antenna_ON();   // Open the antenna to read any RFID tags 
}

// Function to control the state of the RFID CS pin
void RC522_spi_cs_write(bool state) {
	if (state) {
		GPIOB->ODR |= (1UL << 0);
	} else {
		GPIOB->ODR &= ~(1UL << 0);
	}
}

// Function to read a register (8 bits) from the RC522
uint8_t RC522_reg_read8(uint8_t reg) {
	RC522_spi_cs_write(0);
	reg = ((reg << 1) & 0x7E) | 0x80;
	spi_transmit(&reg, 1);
	uint8_t dataRd = 0;
	spi_receive(&dataRd, 1);
	RC522_spi_cs_write(1);
	return dataRd;
}

// Function to write a value (8 bits) to a register in the RC522
void RC522_reg_write8(uint8_t reg, uint8_t data8) {
	RC522_spi_cs_write(0);
	uint8_t txData[2] = { 0x7E & (reg << 1), data8 };
	spi_transmit(txData, 2);
	RC522_spi_cs_write(1);
}

// Function to set a specific bit in a register of the RC522
void RC522_set_bit(uint8_t reg, uint8_t mask) {
	RC522_reg_write8(reg, RC522_reg_read8(reg) | mask);
}

// Function to clear a specific bit in a register of the RC522
void RC522_clear_bit(uint8_t reg, uint8_t mask) {
	RC522_reg_write8(reg, RC522_reg_read8(reg) & (~mask));
}

// Function to reset the RC522
void RC522_reset(void) {
	RC522_reg_write8(0x01, 0x0F);
}

// Function to turn on the antenna for the RC522
void RC522_antenna_ON(void) {
	uint8_t temp;

	temp = RC522_reg_read8(MFRC522_REG_TX_CONTROL); // Output signal on pin TX2
	if (!(temp & 0x03)) {
		RC522_set_bit(MFRC522_REG_TX_CONTROL, 0x03);
	}
}

// Function to check for an RFID card and retrieve its UID
bool RC522_check_card(uint8_t *id) {
	bool status = false;
	// Find cards if tapped against receiver
	status = RC522_request(PICC_REQIDL, id);
	if (status == true) {
		// If card is detected, Card detected
		// Return card UID 4 bytes
		status = RC522_anti_coll(id);
	}
	RC522_halt();      // Command card into hibernation

	return status;
}

// Function to request the RFID card and get its tag type
bool RC522_request(uint8_t reqMode, uint8_t *tagType) {
	bool status = false;
	uint16_t backBits;
	RC522_reg_write8(MFRC522_REG_BIT_FRAMING, 0x07);
	tagType[0] = reqMode;
	status = RC522_to_card(PCD_TRANSCEIVE, tagType, 1, tagType, &backBits);
	if ((status != true) || (backBits != 0x10)) {
		status = false;
	}
	return status;
}

// Function to transmit data to the RFID card and receive its response
bool RC522_to_card(uint8_t command, uint8_t *sendData, uint8_t sendLen,
		uint8_t *backData, uint16_t *backLen) {
	bool status = false;
	uint8_t irqEn = 0x00;
	uint8_t waitIRq = 0x00;
	uint8_t lastBits;
	uint8_t n;
	uint16_t i;

	irqEn = 0x77;
	waitIRq = 0x30;

	RC522_reg_write8(MFRC522_REG_COMM_IE_N, irqEn | 0x80);
	RC522_clear_bit(MFRC522_REG_COMM_IRQ, 0x80);
	RC522_set_bit(MFRC522_REG_FIFO_LEVEL, 0x80);

	RC522_reg_write8(MFRC522_REG_COMMAND, PCD_IDLE);

	// Writing data to the FIFO
	for (i = 0; i < sendLen; i++) {
		RC522_reg_write8(MFRC522_REG_FIFO_DATA, sendData[i]);
	}

	// Execute the command
	RC522_reg_write8(MFRC522_REG_COMMAND, command);
	if (command == PCD_TRANSCEIVE) {
		RC522_set_bit(MFRC522_REG_BIT_FRAMING, 0x80); // StartSend=1, transmission of data starts
	}

	// Waiting to receive data to complete
	i = 100; // i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms???
	do {
		// CommIrqReg[7..0]
		// Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
		n = RC522_reg_read8(MFRC522_REG_COMM_IRQ);
		i--;
	} while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

	RC522_clear_bit(MFRC522_REG_BIT_FRAMING, 0x80);     // StartSend=0

	if (i != 0) {
		if (!(RC522_reg_read8(MFRC522_REG_ERROR) & 0x1B)) {
			status = true;
			if (n & irqEn & 0x01) {
				status = false;
			}

			if (command == PCD_TRANSCEIVE) {
				n = RC522_reg_read8(MFRC522_REG_FIFO_LEVEL);
				uint8_t l = n;
				lastBits = RC522_reg_read8(MFRC522_REG_CONTROL) & 0x07;
				if (lastBits) {
					*backLen = (n - 1) * 8 + lastBits;
				} else {
					*backLen = n * 8;
				}

				if (n == 0) {
					n = 1;
				}
				if (n > MFRC522_MAX_LEN) {
					n = MFRC522_MAX_LEN;
				}

				// Reading the received data in FIFO
				for (i = 0; i < n; i++) {
					uint8_t d = RC522_reg_read8(MFRC522_REG_FIFO_DATA);
					if (l == 4)
						printf("%02x ", d);
					backData[i] = d;
				}
				if (l == 4)
					printf("\r\n");
				return status;
			}
		} else {
			printf("error\r\n");
			status = false;
		}
	}

	return status;
}

// Function to acquire the UID of the RFID card 
bool RC522_anti_coll(uint8_t *serNum) {
	bool status;
	uint8_t i;
	uint8_t serNumCheck = 0;
	uint16_t unLen;

	RC522_reg_write8(MFRC522_REG_BIT_FRAMING, 0x00); // TxLastBists = BitFramingReg[2..0]

	serNum[0] = PICC_ANTICOLL;
	serNum[1] = 0x20;
	status = RC522_to_card(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

	if (status == true) {
		// Check card serial number
		for (i = 0; i < 4; i++) {
			serNumCheck ^= serNum[i];
		}
		if (serNumCheck != serNum[i]) {
			status = false;
		}
	}
	return status;
}

// Function to put the RFID card reader into hibernation until the card has been processed
void RC522_halt(void) {
	uint16_t unLen;
	uint8_t buff[4];

	buff[0] = PICC_HALT;
	buff[1] = 0;
	RC522_calculate_CRC(buff, 2, &buff[2]);

	RC522_to_card(PCD_TRANSCEIVE, buff, 4, buff, &unLen);
}

// Function to calculate the CRC for RFID card communication
void RC522_calculate_CRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData) {
	uint8_t i, n;

	RC522_clear_bit(MFRC522_REG_DIV_IRQ, 0x04);     // CRCIrq = 0
	RC522_set_bit(MFRC522_REG_FIFO_LEVEL, 0x80);      // Clear the FIFO pointer

	// Writing data to the FIFO
	for (i = 0; i < len; i++) {
		RC522_reg_write8(MFRC522_REG_FIFO_DATA, *(pIndata + i));
	}
	RC522_reg_write8(MFRC522_REG_COMMAND, PCD_CALCCRC);

	// Wait CRC calculation is complete
	i = 0xFF;
	do {
		n = RC522_reg_read8(MFRC522_REG_DIV_IRQ);
		i--;
	} while ((i != 0) && !(n & 0x04));      // CRCIrq = 1

	// Read CRC calculation result
	pOutData[0] = RC522_reg_read8(MFRC522_REG_CRC_RESULT_L);
	pOutData[1] = RC522_reg_read8(MFRC522_REG_CRC_RESULT_M);
}
