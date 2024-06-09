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
* @file     security_system_interface.h
* @brief    An interface file declaring the API to check access based on User IDentifier(UID) and passwords.
*
* @author   Krishna Suhagiya and Sriya Garde
* @date     November 30, 2023
* @revision 1.0
*
*/

#ifndef __SECURITY_SYSTEM_H
#define __SECURITY_SYSTEM_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "security_system_interface.h"

/**
 * @brief   A function to check the access to the system based on the UID and passwords.
 *
 * @param   None
 *
 * @return  None.
 */
void check_access(void);

#endif /* __SECURITY_SYSTEM_H */
