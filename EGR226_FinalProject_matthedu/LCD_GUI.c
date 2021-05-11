/***************************************************************************************
 *                              LCD_GUI.c
 *Author        Dustin Matthews
 *Date          7/26/20
 *Instructor    Professor Ekin
 *Description   Contains the various displays to be written to the LCD for the primary menus
 ***************************************************************************************/

#include "LCD_Library.h"
#include "SysTick_Library.h"

/*****************************************************************************************
* Function     startup_display()
* Description  Initial text and instructions on the LCD
*****************************************************************************************/
void startup_display(){

    lcdSetText("MAIN MENU", 0, 0);
    SysTick_delay_ms(1000);
    lcdSetText("MAKE SELECTION", 0, 1);
    lcdSetText("THEN PRESS [#]", 0, 2);
    SysTick_delay_ms(3000);
    lcdClear();
}

/*****************************************************************************************
* Function     main_menu_display()
* Description  Display menu options
*****************************************************************************************/
void main_menu_display(){
    lcdSetText("[1]DOOR MENU", 0, 0);
    lcdSetText("[2]MOTOR MENU", 0, 1);
    lcdSetText("[3]LIGHTS MENU", 0, 2);
}

/*****************************************************************************************
* Function     door_menu_display()
* Description  Display menu options
*****************************************************************************************/
void door_menu_display(){
    lcdSetText("DOOR MENU", 0, 0);
    lcdSetText("[1] TO OPEN", 0, 1);
    lcdSetText("[2] TO CLOSE", 0, 2);
}

/*****************************************************************************************
* Function     motor_menu_display()
* Description  Display menu options
*****************************************************************************************/
void motor_menu_display(){
    lcdSetText("MOTOR MENU", 0, 0);
    lcdSetText("SET SPEED 0-9", 0, 1);
}

/*****************************************************************************************
* Function     lights_menu_display()
* Description  Display menu options
*****************************************************************************************/
void lights_menu_display(){
    lcdSetText("LIGHTS MENU", 0, 0);
    lcdSetText("RED GRN BLU", 0, 1);
    lcdSetText("[1] [2] [3]", 0, 2);
}

