/***************************************************************************************
 *                              Interrupt_Handlers.h
 *Author        Dustin Matthews
 *Date          7/26/20
 *Instructor    Professor Ekin
 *Description   Defines the various interrupt handlers used in the program. These include
 *              port, TA, and ADC types.
 *
 *              Initializes global variables to be used in the ISR's.
 ***************************************************************************************/

#ifndef INTERRUPT_HANDLERS_H_
#define INTERRUPT_HANDLERS_H_

#include "msp.h"
#include <stdlib.h>

void PORT2_IRQHandler();
void PORT5_IRQHandler();
void TA1_0_IRQHandler();
void TA3_0_IRQHandler();
void ADC14_IRQHandler(void);
void T32_INT2_IRQHandler();

/* Duty Cycle RED = RGB_DC[1], GRN = RGB_DC[2], BLU = RGB_DC[3] */
extern float RGB_DC[5];
extern double read;
extern uint16_t lcd_dc;
extern int LED_check;


#endif /* INTERRUPT_HANDLERS_H_ */
