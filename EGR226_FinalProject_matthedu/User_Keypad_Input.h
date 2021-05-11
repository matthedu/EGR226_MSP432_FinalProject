/*
 * User_Keypad_Input.h
 *
 *  Created on: Jul 26, 2020
 *      Author: dstnm
 */

#ifndef USER_KEYPAD_INPUT_H_
#define USER_KEYPAD_INPUT_H_

#include "Keypad_Library.h"
#include "SysTick_Library.h"

float RGB_brightness();
int read_single_digit(int min, int max, int zero_enable);

#endif /* USER_KEYPAD_INPUT_H_ */
