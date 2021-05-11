/***************************************************************************************
 *                              User_Keypad_Input.c
 *Author        Dustin Matthews
 *Date          7/26/20
 *Instructor    Professor Ekin
 *Description   Functions included to read either single digits or a three digit entry
 *              on the keypad. Includes error checking.
 *              Single digit entry will update a single digit array until the user presses
 *              the # key to make their choice.
 *              The RGB_brightness() function will update a three digit array until the
 *              user presses the # key to make their choice.
 ***************************************************************************************/
#include "msp.h"
#include "Keypad_Library.h"
#include "LCD_Library.h"
#include "SysTick_Library.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/***********************************
 *** Char array for keypad input ***
 **********************************/
char keys[14] = {'x', '1', '2', '3', '4', '5', '6', '7', '8', '9', '*',  '0', '#'};

/*****************************************************************************************
 * Function:    read_single_digit()
 * Description: Read the keypad and print each key pressed to the LCD. If the entry is
 *              acceptable, return the value as an integer. If the user pushes the [#] key
 *              on an invalid entry outside the given range, an error message is displayed
 *              and the user must make a new entry. The user can also clear their current
 *              entry with the [*] key.
 * Inputs:      min, max, and zero enable. Min and max allow the function to
 *              accept an entry into the select array only if it is in the specified range.
 *              The [0] key lies between the [*] and [#] keys, so the zero enable variable
 *              allows the function to accept a press of the [0] key into the select array.
 *
*****************************************************************************************/
int read_single_digit(int min, int max, int zero_enable){
    int i = 0, val = -1;
    char select[2] = {'0', '\0'};               //array to store the last valid keypad press
       while(1){
           i = read_keypad();
           if (i > 0)
               lcdSetChar(keys[i], 0, 3);       //write entry to the LCD
           if ((i > 0) && (i != 10) && (i != 12)){
               select[0] = keys[i];
               val = atoi(select);              //store the value as an integer
           }
           else if ((zero_enable) && (i == 11)){
               select[0] = '0';
               val = atoi(select);
           }
           if (i == 10){                            // [*] to clear entry
               memset(select, '0', strlen(select)); //clear the array
               val = atoi(select);
               lcdSetText("CLEARING...", 0, 3);
               SysTick_delay_ms(500);
               lcdSetText("           ", 0, 3);
           }
           if ((i == 12) && (val >= min) && (val < max)){   //value is in range
             lcdClear();
             lcdSetText("YOU ENTERED", 0, 0);
             lcdSetChar(select[0], 0, 1);
             SysTick_delay_ms(2000);
             lcdClear();
             return val;                                    //return the user selection
           }
           else if ((i == 12) && (val < min) | (val >= max)){
               lcdSetText("INVALID ENTRY", 0, 3);
               SysTick_delay_ms(2000);
               lcdSetText("             ", 0, 3);
           }
       }
}

float RGB_brightness(){
    int i = 0, brite;
    char RGB[4] = {'0', '0', '0', '\0'};

    while(1){
        i = read_keypad();
        if ((i > 0) && (i != 10) && (i != 12)){
            RGB[0] = RGB[1];
            RGB[1] = RGB[2];
            RGB[2] = keys[i];
            brite = atoi(RGB);
            lcdSetInt(brite, 0, 3);
        }
        if (i == 10){                           // [*] to clear entry
            memset(RGB, '0', strlen(RGB));
            brite = atoi(RGB);
            lcdSetText("CLEARING...", 0, 3);
            SysTick_delay_ms(500);
            lcdSetText("           ", 0, 3);
        }
        if ((i == 12) && (brite <= 100)){
            lcdClear();
            SysTick_delay_ms(500);
            lcdSetText("LED LVL: ", 0, 0);
            lcdSetInt(brite, 0, 1);
            SysTick_delay_ms(1500);
            lcdClear();
            return brite;
        }
        else if ((i == 12) && (brite > 100)){
            lcdSetText("ERROR- TRY AGAIN", 0, 3);
            memset(RGB, '0', strlen(RGB));
            brite = atoi(RGB);
            SysTick_delay_ms(1000);
            lcdSetText("                ", 0, 3);
        }
    }
}

