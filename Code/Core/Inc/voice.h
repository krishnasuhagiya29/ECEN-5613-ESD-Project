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
* @file   voice.h
* @brief  A file declaring the APIs for voice recorder and speaker module.
*
* @author Sriya Garde
* @date   November 25, 2023
* @revision 1.0
*
*/

#ifndef __VOICE_H_
#define __VOICE_H_

/**
 * @brief   A function to initialize voice recorder and speaker module.
 *
 * @param   None
 *
 * @return  None.
 */
void voice_init(void);

/**
 * @brief   A function to output the recorded message.
 *
 * @param   None
 *
 * @return  None.
 */
void voice_check(void);

#endif /* __VOICE_H_ */
