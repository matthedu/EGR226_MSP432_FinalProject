/***************************************************************************************
 *                              TIMER_A_Library.c
 *Author        Dustin Matthews
 *Date          7/26/20
 *Instructor    Professor Ekin
 *Description   Functions included to initialize Timer A channels and (if applicable)
 *              their associated pins.
 ***************************************************************************************/

#include <msp.h>
#include <SysTick_Library.h>

/*****************************************************************************************
 * Function:    init_TAO_Pins()
 * Description: Set up three channels of TimerA0 to drive PWM levels for the RGB LED module
*****************************************************************************************/
void init_TA0_Pins(){

    /* TA0 */
    TIMER_A0->CTL = 0b0000001000010100;         //SMCLK | UP Mode | TACLR
    TIMER_A0->CCR[0] = 3000;                    //PWM Period (1000ms / 1 sec) * (1 sec / 1000Hz ) = 1ms Period
                                                //(3000 clock cycles / second) * 1ms = 3000 clock ticks per Period
    /* PWM Pins for RGB LED Module */
    uint8_t pin_mask = 0b11100000;
    P2->SEL1 &= ~pin_mask;
    P2->SEL0 |= pin_mask;
    P2->DIR |= pin_mask;

    /* Red TA0.2 */
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7; //Output mode-> Reset/set
    TIMER_A0->CCR[2] = 0;                       //RED Duty Cycle - Start 0%

    /* Green TA0.3 */
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7; //Output mode-> Reset/set
    TIMER_A0->CCR[3] = 0;                       //GRN Duty Cycle - Start 0%

    /* Blue TA0.4 */
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7; //Output mode-> Reset/set
    TIMER_A0->CCR[4] = 0;                       //BLU Duty Cycle - Start 0%
}

/*****************************************************************************************
*Function:      init_TA1()
*Description:   Set up TA1 for quarter second interrupts using SMCLK in UP mode
*               SMCLK default 3MHz -> /64 to get 46.875 clock ticks per ms
*               46.875 * 250ms = 11718 period, load this into CCR[0]
*               Use CCTL[0] interrupt
*****************************************************************************************/
void init_TA1(){

        TIMER_A1->CCR[0] = 11718;
        TIMER_A1->CTL = 0b0000001011000000;     //Source: SMCLK, Halted, ID /8
        TIMER_A1->EX0 = 0b0000000000000111;     //divide clock further by 8
        TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE;
}

/*****************************************************************************************
 * Function:    init_TA2_Pins()
 * Description: Set up three channels of TimerA2 to drive PWM levels for the DC motor,
 *              Servo, and LCD brightness
*****************************************************************************************/
void init_TA2_Pins(){

    /* TA2 */
    TIMER_A2->CTL = 0b0000001000010100;         //SMCLK | UP Mode | TACLR

    TIMER_A2->CCR[0] = 60000;                   //PWM Period (1000ms / 1 sec) * (1 sec / 50Hz ) = 20ms Period
                                                //(3000 clock cycles / second) * 20ms = 60000 clock ticks per Period
    /* PWM Pin for DC Motor,  */
    P5->SEL1 &= ~BIT7;
    P5->SEL0 |= BIT7;
    P5->DIR |= BIT7;
    P5->OUT &= ~BIT7;

    /* Motor Warning LED Pin */
    P5->SEL0 &= ~BIT0;                          //Set for GPIO
    P5->SEL1 &= ~BIT0;
    P5->DIR |= BIT0;                            //Set for output
    P5->OUT &= ~BIT0;                           //start with LED off

    /* TA2.2 */
    TIMER_A2->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7; //Output mode-> Reset/set
    TIMER_A2->CCR[2] = 0;                       //DC Motor Duty Cycle - Start 0%

    /* PWM Pin and R/G LEDs for Servo Door */
    P6->SEL1 &= ~BIT6;
    P6->SEL0 |= BIT6;
    P6->DIR |= BIT6;                            //PWM PIN

    P3->SEL0 &= ~(BIT6 | BIT7);
    P3->SEL1 &= ~(BIT6 | BIT7);
    P3->DIR |= (BIT6 | BIT7);                   //R/G LEDs
    P3->OUT &= ~BIT6;                           //turn off Green LED

    /* TA2.3 */
    TIMER_A2->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7; //Output mode-> Reset/set
    TIMER_A2->CCR[3] = 6000;                    //Servo Duty Cycle - Start Closed
    SysTick_delay_ms(100);
    TIMER_A2->CCR[3] = 0;                       //Set Duty Cycle to 0

    P2->OUT |= BIT0;                            //Turn on RED LED to indicate door closed

    /* PWM Pin for LCD Brightness */
    P6->SEL1 &= ~BIT7;
    P6->SEL0 |= BIT7;
    P6->DIR |= BIT7;

    /* TA2.4 */
    TIMER_A2->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7; //Output mode-> Reset/set
    TIMER_A2->CCR[4] = 45000;                   //LCD Brightness Duty Cycle - Start 75%
}

/*****************************************************************************************
 * Function:    TIMER_A3_delay()
 * Description: Set up TimerA3 with interrupt to use as a 10ms debounce of the port buttons
*****************************************************************************************/
void TIMER_A3_delay(){

    TIMER_A3->CTL = 0b0000001000000100;         //SMCLK | HALTED | TACLR
    TIMER_A3->CCTL[0] = TIMER_A_CCTLN_CCIE;     //Interrupts enabled
    TIMER_A3->CCR[0] = 30000;                   //debounce 10ms: 3000 cycles per ms * 10 ms
}

