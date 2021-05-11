/***************************************************************************************
 *                              keypad_read.c
 *Author        Dustin Matthews
 *Date          7/26/20
 *Instructor    Professor Ekin
 *Description   Reads the keypad and returns values 1 - 13 on a successful read
 ***************************************************************************************/

//------------------------------------------------------ Library Includes
#include "Keypad_Library.h"
#include "SysTick_Library.h"
#include "msp.h"
#include <stdlib.h>
#include <stdio.h>
//------------------------------------------------------ Library Includes
/*
 * Row 1:       P3.0
 * Row 2:       P3.2
 * Row 3:       P3.3
 * Row 4:       P3.5
 *
 * Column 1:    P1.5
 * Column 2:    P1.6
 * Column 3:    P1.7
 */
int read_keypad(){
uint8_t col, row;

for(col = 5; col < 8; col++){
    P1->DIR &= ~(BIT5 | BIT6 | BIT7);       // set column bits to inputs
    P1->DIR |= (1<<col);                    //set column pin as output
    P1->OUT &= ~(1<<col);                   //set column pin as LOW
    SysTick_delay_ms(15);                   //configure pins/ debounce

    row = P3->IN & (0b00101101);

/* wait for buttons to be released */
    while (!(P3->IN & BIT0) | !(P3->IN & BIT2) | !( P3->IN & BIT3) | !( P3->IN & BIT5));

    if (row != 0b00101101)
        break;
}
    P1->DIR &= ~(BIT5 | BIT6 | BIT7);       //return all columns to input

    if (col == 8)
            return 0;                       //no keys read, return 0
/* return an integer that corresponds to the key in the keypad array */
    if (row == 0b00101100) return (col - 5) + 1;
    if (row == 0b00101001) return 3 + (col - 5) + 1;
    if (row == 0b00100101) return 6 + (col - 5) + 1;
    if (row == 0b00001101) return 9 + (col - 5) + 1;

/* return -1 if an error has occurred */
    return -1;
}




