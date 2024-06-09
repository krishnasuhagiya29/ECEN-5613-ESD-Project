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
* @file   rfid.h
* @brief  A file declaring the RFID (RC522) related APIs.
*
* @author Krishna Suhagiya and Sriya Garde
* @date   November 8, 2023
* @reference https://github.com/Hamid-R-Tanhaei/RFID-MIFARE-RC522-ARM-STM32/
* @revision 1.0
*
*/

#ifndef __RFID_H
#define __RFID_H

#include "stdbool.h"
#include "stdint.h"

/* MFRC522 Commands */
#define PCD_IDLE            0x00   //NO action; Cancel the current command
#define PCD_AUTHENT           0x0E   //Authentication Key
#define PCD_TRANSCEIVE          0x0C   //Transmit and receive data,
#define PCD_CALCCRC           0x03   //CRC Calculate

/* Mifare_One card command word */
#define PICC_REQIDL           0x26   // find the antenna area does not enter hibernation
#define PICC_ANTICOLL         0x93   // anti-collision
#define PICC_HALT           0x50   // Sleep

/* MFRC522 Registers */
//Page 0: Command and Status
#define MFRC522_REG_COMMAND       0x01
#define MFRC522_REG_COMM_IE_N     0x02
#define MFRC522_REG_COMM_IRQ      0x04
#define MFRC522_REG_DIV_IRQ       0x05
#define MFRC522_REG_ERROR       0x06
#define MFRC522_REG_FIFO_DATA     0x09
#define MFRC522_REG_FIFO_LEVEL      0x0A
#define MFRC522_REG_CONTROL       0x0C
#define MFRC522_REG_BIT_FRAMING     0x0D
//Page 1: Command
#define MFRC522_REG_MODE        0x11
#define MFRC522_REG_TX_CONTROL      0x14
#define MFRC522_REG_TX_AUTO       0x15
#define MFRC522_REG_CRC_RESULT_M    0x21
#define MFRC522_REG_CRC_RESULT_L    0x22
#define MFRC522_REG_T_MODE        0x2A
#define MFRC522_REG_T_PRESCALER     0x2B
#define MFRC522_REG_T_RELOAD_H      0x2C
#define MFRC522_REG_T_RELOAD_L      0x2D

#define MFRC522_MAX_LEN         16

/**
 * @brief   A function to initialize RC522 RFID module.
 *
 * @param   None
 *
 * @return  None.
 */
void RC522_init(void);

/**
 * @brief   A function to control the Chip Select (CS) pin of the RC522 module.
 *
 * @param   state to set pin low/high
 *
 * @return  None.
 */
void RC522_spi_cs_write(bool state);

/**
 * @brief   A function to read an 8-bit register from the RC522 RFID/NFC module using SPI communication.
 *
 * @param   Register address
 *
 * @return  Received data.
 */
uint8_t RC522_reg_read8(uint8_t reg);

/**
 * @brief   A function to write an 8-bit value to a specific register in the RC522 RFID/NFC module using SPI communication.
 *
 * @param   reg Register address
 *          data8 An 8-bit value to write
 *
 * @return  Data value.
 */
void RC522_reg_write8(uint8_t reg, uint8_t data8);

/**
 * @brief   A function to set specific bits in a register of the RC522 RFID/NFC module.
 *
 * @param   reg Register address
 *          mask Mask value
 *
 * @return  Data value.
 */
void RC522_set_bit(uint8_t reg, uint8_t mask);

/**
 * @brief   A function to clear specific bits in a register of the RC522 RFID/NFC module.
 *
 * @param   reg Register address
 *          mask Mask value
 *
 * @return  None.
 */
void RC522_clear_bit(uint8_t reg, uint8_t mask);

/**
 * @brief   A function to reset the RC522 RFID/NFC module.
 *
 * @param   None
 *
 * @return  None.
 */
void RC522_reset(void);

/**
 * @brief   A function to turn on the antenna of the RC522 RFID/NFC module by configuring the TX_CONTROL register.
 *
 * @param   None
 *
 * @return  None.
 */
void RC522_antenna_ON(void);

/**
 * @brief   A function to check for the presence of a card and retrieves its Unique IDentifier (UID) if a card is detected.
 *
 * @param   id Pointer the card UID.
 *
 * @return  Card checking result.
 */
bool RC522_check_card(uint8_t *id);

/**
 * @brief   A function part of the process of making a request to a nearby RFID/NFC card using the RC522 module.
 * It initiates the communication and requests the card to respond, then checks the response to determine if a card has been detected.
 *
 * @param   reqMode Request mode.
 *          tagType Pointer to the tag type array.
 *
 * @return  Request result.
 */
bool RC522_request(uint8_t reqMode, uint8_t *tagType);

/**
 * @brief   A function to send a command to the card and receive the response data.
 *
 * @param   command  command
 *          sendData Pointer to the data to send.
 *          sendLen  Length of the data to send.
 *          backData Pointer to the response data.
 *          backLen  Lenth of the response data.
 *
 * @return  Result.
 */
bool RC522_to_card(uint8_t command, uint8_t *sendData, uint8_t sendLen,
		uint8_t *backData, uint16_t *backLen);

/**
 * @brief   A function of anti-collision anti-collision to detect and select a specific card when multiple cards are present in the reader's field.
 *
 * @param   serNum Pointer to the serial number array for anti-collision
 *
 * @return  Status of the anti-collision process.
 */
bool RC522_anti_coll(uint8_t *serNum);

/**
 * @brief   A function to halt communication with an RFID/NFC card using the RC522 module.
 *
 * @param   None
 *
 * @return  None.
 */
void RC522_halt(void);

/**
 * @brief   A function to calculate the CRC (Cyclic Redundancy Check) for a given set of input data using the RC522 RFID/NFC module.
 *
 * @param   pIndata Pointer to the input data
 *          len     Length of the input data
 *          pOutData Pointer to the output data
 *
 * @return  None.
 */
void RC522_calculate_CRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData);

#endif /* __RFID_H */
