/***************************************************************************************
 *                              LCD_GUI.h
 *Author        Dustin Matthews
 *Date          7/26/20
 *Instructor    Professor Ekin
 *Description   Contains the various displays to be written to the LCD for the primary menus
 ***************************************************************************************/


#ifndef LCD_GUI_H_
#define LCD_GUI_H_

#include "msp.h"
#include "LCD_Library.h"

void startup_display();
void main_menu_display();
void door_menu_display();
void motor_menu_display();
void lights_menu_display();

#endif /* LCD_GUI_H_ */
