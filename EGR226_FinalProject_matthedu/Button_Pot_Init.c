/***************************************************************************************
 *                              Button_Pot_Init.c
 *Author        Dustin Matthews
 *Date          7/26/20
 *Instructor    Professor Ekin
 *Description   Functions included to initialize pins utilizing interrupts activated by
 *              external components, i.e. buttons and a potentiometer.
 ***************************************************************************************/

#include "msp.h"
#include "Button_Pot_Init.h"

/*****************************************************************************************
 * Function:    init_buttons()
 * Description: Set up button ports for GPIO and port interrupts
*****************************************************************************************/
void init_buttons(){

    /* LED Port Interrupt Button P2.3 */
    P2->SEL1 &= ~BIT3;              //Set as GPIO
    P2->SEL0 &= ~BIT3;              //Set as GPIO
    P2->DIR &= ~BIT3;               //Set buttons as input
    P2->REN |= BIT3;                //Enable Internal resistance
    P2->OUT |=BIT3;                 //Set pull-up resistor
    P2->IES |=BIT3;                 //Set pin interrupt to trigger at 1>0 transition
    P2->IE |= BIT3;                 //Enable interrupt for button pins
    P2->IFG = 0;                    //Clear all P6 interrupt flags

    /* Motor Port Interrupt Button P5.5 */
    P5->SEL1 &= ~BIT5;              //Set as GPIO
    P5->SEL0 &= ~BIT5;              //Set as GPIO
    P5->DIR &= ~BIT5;               //Set buttons as input
    P5->REN |= BIT5;                //Enable Internal resistance
    P5->OUT |=BIT5;                 //Set pull-up resistor
    P5->IES |=BIT5;                 //Set pin interrupt to trigger at 1>0 transition
    P5->IE |= BIT5;                 //Enable interrupt for button pins
    P5->IFG = 0;                    //Clear all P5 interrupt flags

}

/*****************************************************************************************
 * Function:    LCD_Pot()
 * Description: Set up ADC15 on Pin P6.0 with interrupts to read the potentiometer when
 *              a conversion is made.
*****************************************************************************************/
void LCD_Pot(){

    /* Read Pin for the Potentiometer (A15) */
    P6->SEL1 |= BIT0;                           //Tertiary Function
    P6->SEL0 |= BIT0;                           //Tertiary Function
    P6->DIR &=~ BIT0;                           //Configure for input

    /* Initialize the ADC14 to read the potentiometer */
    ADC14->CTL0 &=~ ADC14_CTL0_ENC;             //disable ADC converter
    ADC14->CTL0 |= 0x44540210;                  //Set/hold pulse mode, SMCLK, 16clks, continuous sampling, ADC on
    ADC14->CTL1 = 0x000F0030;                   //14 bit resolution, store in mem15 register
    ADC14->MCTL[15] =0x0000000F;                //channel 15 for mem15 interrupts
    ADC14->IER0 |= 0x00008000;                  //enable interrupts on mem15
    ADC14->CTL0 |= ADC14_CTL0_ENC;              //enable ADC14
}
