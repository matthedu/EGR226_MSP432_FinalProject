/***************************************************************************************
 *                              TIMER_A_Library.h
 *Author        Dustin Matthews
 *Date          7/26/20
 *Instructor    Professor Ekin
 *Description   Functions included to initialize Timer A channels and (if applicable)
 *              their associated pins.
 ***************************************************************************************/

#ifndef TIMER_A_LIBRARY_H_
#define TIMER_A_LIBRARY_H_

#include "msp.h"

void init_TA0_Pins();
void init_TA1();
void init_TA2_Pins();
void TIMER_A3_delay();


#endif /* TIMER_A_LIBRARY_H_ */
