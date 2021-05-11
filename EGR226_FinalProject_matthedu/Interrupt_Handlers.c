/***************************************************************************************
 *                              Interrupt_Handlers.c
 *Author        Dustin Matthews
 *Date          7/26/20
 *Instructor    Professor Ekin
 *Description   Defines the various interrupt handlers used in the program. These include
 *              port, TA, and ADC types.
 ***************************************************************************************/
#include "msp.h"
#include "Interrupt_Handlers.h"
#include <stdlib.h>
#include <stdio.h>

/***********************************
 ***** Define global variables *****
 **********************************/
float RGB_DC[5] = {0, 0, 0, 0};         //duty cycles for the RGB module
double read = 0;                        //reading from the ADC potentiometer
uint16_t lcd_dc = 0;                    //duty cycle for the LCD
int LED_check = 0;                      //check variable for LED button
int screen_save_digit = 0;

/*****************************************************************************************
 * Function     PORT5_IRQHandler()
 * Description  Trigger TA3 interrupt to debounce P2.3 button and toggle the RGB LED module
 *****************************************************************************************/
void PORT2_IRQHandler(){

    TIMER_A3->CTL |= BIT4;                      //Set TA3 to UP MODE
    TIMER_A3->CTL |= TIMER_A_CTL_CLR;           //restart TA3 count
    P2->IFG = 0;                                //reset all P2 flags
}


/*****************************************************************************************
* Function     PORT5_IRQHandler()
* Description  Trigger TA3 interrupt to debounce buttons on P5.5 or P2.3
*****************************************************************************************/
void PORT5_IRQHandler(){

    TIMER_A3->CTL |= BIT4;                      //Set TA3 to UP MODE
    TIMER_A3->CTL |= TIMER_A_CTL_CLR;           //restart TA3 count
    P5->IFG = 0;                                //reset all P5 flags
}

/*****************************************************************************************
* Function      TA1_0_IRQHandler()
* Description   Activated when duty cycle of the DC motor reaches or exceeds 80%, toggle an
*               LED every .25 seconds.
*****************************************************************************************/
void TA1_0_IRQHandler(){
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  //clear interrupt flag
    P5->OUT ^= BIT0;                            //toggle warning LED
}

/*****************************************************************************************
* Function      TA3_0_IRQHandler()
* Description   Debounce timer for buttons.
*               If the DC motor killswitch is pressed, set the Duty Cycle to 0.
*
*               If the RGB toggle button is pressed, manipulate the CCRn registers
*               to toggle the RGB module while retaining the duty cycle values, which
*               are saved as global variables.
*****************************************************************************************/
void TA3_0_IRQHandler(){
    int i;

    TIMER_A3->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  //clear interrupt flag
    if((P5->IN & BIT5) == 0){                   //If switch is still low, update press variable
        TIMER_A2->CCR[2] = 0;                   //Set DC motor duty cycle to 0
        TIMER_A1->CTL &= ~BIT4;                 //disable warning LED Timer
        P5->OUT &= ~BIT0;                       //turn off warning LED
    }
    if((P2->IN & BIT3) == 0){                   //RGB button pressed, manipulate CCRn registers
        if (LED_check){                         //if any LEDs have been set, toggle all off
            for (i = 2; i < 5; i++){
                TIMER_A0->CCR[i] = 0;
            }
            LED_check = 0;
        }
        else{
            for (i = 2; i < 5; i++){            //else toggle LEDs all on
            TIMER_A0->CCR[i] = RGB_DC[i - 1];
            }
            LED_check = 1;
        }

    }
    TIMER_A3->CTL &= ~BIT4;                     //Halt TA3
    TIMER_A3->CTL |= TIMER_A_CTL_CLR;           //Clear TA3R count
}

/*****************************************************************************************
 * Function     ADC14_IRQHandler()
 * Description  Read potentiometer if a conversion is made, adjust LCD brightness
 *****************************************************************************************/
void ADC14_IRQHandler(void){

    ADC14->CLRIFGR0 |= 0x00008000;
    read = ADC14->MEM[15];                      //read mem[15] register
    lcd_dc = (read / 16384) * 60000;            //calculate new duty cycle using the 14-bit resolution value
    if (lcd_dc < 100)                           //if duty cycle < 100, turn off LCD (avoids sputtering activation)
        lcd_dc = 0;
    TIMER_A2->CCR[4] = lcd_dc;                  //adjust LCD brightness
    ADC14->CTL0 |= ADC14_CTL0_SC;
}


