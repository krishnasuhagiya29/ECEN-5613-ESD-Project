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
* @file   security_system_interface.c
* @brief  A file defining the API to check access based on User IDentifier(UID) and passwords.
*
* @author Krishna Suhagiya and Sriya Garde
* @date   November 30, 2023
* @revision 1.0
*
*/

#include <stdio.h>
#include "security_system_interface.h"
#include "rfid.h"
#include "oled.h"
#include "keypad.h"
#include "beeper.h"
#include "voice.h"
#include "UART.h"

//Defining fields for checking Valid and Invalid cards
#define VALID_CARDS	2
#define TOTAL_CARDS	4
#define UID_LENGTH	10
#define PASSWORD_LENGTH	5
#define MAX_INPUT_LENGTH	20
uint8_t rfid_id[TOTAL_CARDS] = { 0 };

//Defining char arrays for card UIDs
char *myTags[VALID_CARDS] = { };

char uid_1[UID_LENGTH] = "e39a9fb";
char uid_2[UID_LENGTH] = "23a2a2c5";
char uid_3[UID_LENGTH] = { 0 };
char uid_4[UID_LENGTH] = { 0 };

bool add_tag = true;
char admin_password[PASSWORD_LENGTH] = "1234";
char security_password[PASSWORD_LENGTH] = "5678";
char received_string[MAX_INPUT_LENGTH];
int i = 0;
unsigned char tagindex = 0;

char buffer[MAX_INPUT_LENGTH];

void check_access(void) {
	//Checking if a card is tapped against the RFID reader
	if (RC522_check_card(rfid_id)) {
	//Extracting the UID of the tapped card.
		sprintf(buffer, "%x%x%x%x", rfid_id[0], rfid_id[1], rfid_id[2],
				rfid_id[3]);
#ifdef DEBUG
		USART2_string_transmit("\r\n");
#endif
	//Validating the obtained UID of the tapped card against the valid cards saved in the system
		if ((strcmp(buffer, uid_1) == 0) || (strcmp(buffer, uid_2) == 0)
				|| (strcmp(buffer, uid_3) == 0)
				|| (strcmp(buffer, uid_4) == 0)) {
#ifdef DEBUG
			USART2_string_transmit("Access Granted \r\n");
#endif
		//Displaying Access Granted on the OLED.
			SSD1106_gotoXY(0, 0);
			SSD1106_puts("  Access Granted  ", &Font_7x10, 1);
			SSD1106_gotoXY(0, 10);
			SSD1106_clear_line();
			SSD1106_gotoXY(0, 20);
			SSD1106_clear_line();
			SSD1106_update_screen(); //display
			beeper_enable();
			delay(50);
		}

		else {
#ifdef DEBUG
			USART2_string_transmit("Card does not exist.\r\n");
#endif
		//Displaying Card doesnot exist on the LED
			SSD1106_gotoXY(0, 0);
			SSD1106_puts("Card doesn't exist", &Font_7x10, 1);
		//Playing Access Denied message on the Playback module
			voice_check();
#ifdef DEBUG
			USART2_string_transmit(
					"Please enter 4 digit admin password for security pass\r\n");
#endif	//Taking security password input from the user using the Keypad and displaying on OLED
			SSD1106_gotoXY(0, 10);
			SSD1106_puts("   Please enter   ", &Font_7x10, 1);
			SSD1106_gotoXY(0, 20);
			SSD1106_puts("security password:", &Font_7x10, 1);
			SSD1106_update_screen(); //display
			strcpy(received_string, check_key());

		//Checking if the correct Security password has been entered and displaying "Access Granted" if it's correct. 
			if (strcmp(security_password, received_string) == 0) {
				SSD1106_gotoXY(0, 0);
				SSD1106_puts("  Access Granted  ", &Font_7x10, 1);
				SSD1106_gotoXY(0, 10);
				SSD1106_clear_line();
				SSD1106_gotoXY(0, 20);
				SSD1106_clear_line();
				SSD1106_update_screen(); //display
		//Buzzer ON if access is granted
				beeper_enable();

		//If the incorrect security password has been entered, get input displau "Security password wrong" on OLED
			} else if (strcmp(security_password, received_string) != 0) {
				SSD1106_gotoXY(0, 0);
				SSD1106_puts("Security password ", &Font_7x10, 1);
				SSD1106_gotoXY(0, 10);
				SSD1106_puts("       wrong      ", &Font_7x10, 1);
				SSD1106_gotoXY(0, 20);
				SSD1106_clear_line();
				SSD1106_update_screen(); //display
				//Playing Access Denied message on the Playback module
				voice_check();
				//Checking if Valid cards in database is equal to total cards 
				for (unsigned char j = 0; j < VALID_CARDS; j++) {
					if (strcmp(buffer, myTags[j]) == 0) {
						add_tag = false;
						break; //// If valid cards = total cards , we cannot add more cards to the system
					}
					add_tag = true; // If valid cards > total cards , we can add more cards to the system
				}

			//If a card is valid and valid cards < total cards, accept user input for Admin password to add a card from the Keypad
				if ((i < VALID_CARDS) && (add_tag == true)) {
#ifdef DEBUG

			//Displaying "Please enter admin password" on OLED
					USART2_string_transmit(
							"Please enter 4 digit admin password for adding a card\r\n");
#endif
					SSD1106_gotoXY(0, 0);
					SSD1106_puts("  Please enter    ", &Font_7x10, 1);
					SSD1106_gotoXY(0, 10);
					SSD1106_puts("  admin password  ", &Font_7x10, 1);
					SSD1106_gotoXY(0, 20);
					SSD1106_puts("  to add a card:  ", &Font_7x10, 1);
					SSD1106_update_screen(); //display

				//Validate admin password, and if correct, add the card as a valid card to the system
					if (strcmp(admin_password, check_key()) == 0) {
						myTags[i] = strdup(buffer);
						if (i == 0) {
							strcpy(uid_3, buffer);
						}
						if (i == 1) {
							strcpy(uid_4, buffer);
						}
#ifdef DEBUG

				//Display "Adding an access card" on OLED
						USART2_string_transmit("Adding an access card\r\n");
#endif
						SSD1106_gotoXY(0, 0);
						SSD1106_puts("    Card added    ", &Font_7x10, 1);
						SSD1106_gotoXY(0, 10);
						SSD1106_clear_line();
						SSD1106_gotoXY(0, 20);
						SSD1106_clear_line();
						SSD1106_update_screen(); //display
#ifdef DEBUG
						USART2_string_transmit("\r\n");
#endif
						i++;
				//Checking the valid cards database for a match for the tapped card
						for (unsigned char j = 0; j < VALID_CARDS; j++) {
							if (strcmp(buffer, myTags[j]) == 0) {
								tagindex = j;
								break;
							}
						}

			// Granting access to Valid cards by checking UIDs from system database. 
			//Giving access to the Valid card by displaying "Access granted" and beeping buzzer
						if (tagindex < VALID_CARDS) {
#ifdef DEBUG
							USART2_string_transmit("Access granted\r\n");
#endif
							SSD1106_gotoXY(0, 0);
							SSD1106_puts("  Access Granted  ", &Font_7x10, 1);
							SSD1106_gotoXY(0, 10);
							SSD1106_clear_line();
							SSD1106_gotoXY(0, 20);
							SSD1106_clear_line();
							SSD1106_update_screen(); //display
							beeper_enable();
						} else {
#ifdef DEBUG

			//Rejecting access to the Invalid card by displaying "Access Rejected" and playing audio on playback module
							USART2_string_transmit("Access rejected\r\n");
							USART2_string_transmit("Please try again\r\n");
#endif
							SSD1106_gotoXY(0, 0);
							SSD1106_puts("  Access Denied   ", &Font_7x10, 1);
							SSD1106_gotoXY(0, 10);
							SSD1106_puts("     Try again.   ", &Font_7x10, 1);
							SSD1106_gotoXY(0, 20);
							SSD1106_clear_line();
							SSD1106_update_screen(); //display
							voice_check();
						}

				//If Admin password entered is wrong, display "Access Denied" on OLED and ask user to start process again

					} else {
#ifdef DEBUG
						USART2_string_transmit(
								"Admin password wrong.Access Denied\r\n");
#endif
						SSD1106_gotoXY(0, 0);
						SSD1106_puts("  Admin password  ", &Font_7x10, 1);
						SSD1106_gotoXY(0, 10);
						SSD1106_puts("      wrong.      ", &Font_7x10, 1);
						SSD1106_gotoXY(0, 20);
						SSD1106_puts("  Access Denied   ", &Font_7x10, 1);
						SSD1106_update_screen(); //display
						voice_check();

					}

				}

				delay(100);

		// If flow is disrupted at any point, system goes into this loop and states "Acces Denied", "Try again."
			} else {
#ifdef DEBUG
				USART2_string_transmit("Admin password wrong.Access Denied\r\n");
				USART2_string_transmit("Please try again\r\n");
#endif
				SSD1106_gotoXY(0, 0);
				SSD1106_puts("  Admin password  ", &Font_7x10, 1);
				SSD1106_gotoXY(0, 10);
				SSD1106_puts("      wrong.      ", &Font_7x10, 1);
				SSD1106_gotoXY(0, 20);
				SSD1106_puts("  Access Denied   ", &Font_7x10, 1);
				SSD1106_gotoXY(0, 30);
				SSD1106_puts("    Try again.    ", &Font_7x10, 1);
				SSD1106_update_screen(); //display
				voice_check();
			}
		}
		delay(100);
	}
}

