/***************************************************************************************
 *                              keypad_init.c
 *Author        Dustin Matthews
 *Date          7/26/20
 *Instructor    Professor Ekin
 *Description   Initializes the pins for reading the keypad
 ***************************************************************************************/

/*Keypad initialization function.
 * Row 1:       P3.0
 * Row 2:       P3.2
 * Row 3:       P3.3
 * Row 4:       P3.5
 *
 * Column 1:    P1.5
 * Column 2:    P1.6
 * Column 3:    P1.7
 */

//------------------------------------------------------ Library Includes
#include "Keypad_Library.h"
#include "msp.h"
//------------------------------------------------------ Library Includes

void keypad_init(){

uint8_t col_mask = 0b11100000;      //mask for column keypad pins
uint8_t row_mask = 0b00101101;      //mask for row keypad pins

/* Set up Columns*/
P1->SEL1 &= ~col_mask;              //clear col bits for GPIO
P1->SEL0 &= ~col_mask;              //clear col for GPIO
P1->DIR &= ~col_mask;               //clear all col bits initially for input

/* Set up Rows */
P3->SEL1 &= ~row_mask;              //clear row bits for GPIO
P3->SEL0 &= ~row_mask;              //clear row bits for GPIO
P3->DIR &= ~row_mask;               //clear row bits initially for input
P3->REN |= row_mask;                //enable resistors on row pins
P3->OUT |= row_mask;                //enable pull-up resistors on row pins

}
