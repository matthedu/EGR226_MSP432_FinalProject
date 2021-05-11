/*******************************************************************************************************
 *Author        Dustin Matthews
 *Course        EGR 226: Microcontroller Programming and Applications
 *Assignment    Final Project
 *Date          7/26/20
 *Instructor    Professor Ekin
 *File          EGR226_FinalProject_Matthews
 *Description   This program uses an LCD to act as a GUI consisting of
 *              four basic menus: Main, Door, Motor, and Lights. The user
 *              navigates the menus and interacts with the program using a
 *              keypad. After each action the program returns to the main menu.
 *
 *              The LCD brightness can be adjusted at any time with a potentiometer,
 *              which is tied to an ADC15 interrupt. The potentiometer's level is used to set a
 *              PWM with a Timer A instance to adjust the LCD's brightness.
 *
 *              The door menu allows the user to open or close the door,
 *              which will toggle either a green LED (open) or a red LED
 *              (closed).
 *
 *              The motor menu allows the user to set the speed of a DC motor
 *              (0-9). The DC motor can be stopped at any time using a kill
 *              switch, which is tied to a port interrupt and sets the duty
 *              cycle to 0. If the motor duty cycle is at or above 80%, a warning
 *              LED will blink every .25 seconds until is goes below the 80% level.
 *
 *              The lights menu allows the user to select a red, green, or blue
 *              LED on an RGB module, and then set the brightness of the LED
 *              from 0-100. A button will toggle the LEDs on or off at anytime,
 *              retaining their previously set brightness level.
 *
 *Notes:        Various functions are stored in the attached libraries to make the main.c file easier to read
 *              and navigate.
 ********************************************************************************************************/
#include "msp.h"
#include "LCD_Library.h"
#include "Keypad_Library.h"
#include "SysTick_Library.h"
#include "LCD_GUI.h"
#include "User_Keypad_Input.h"
#include "TIMER_A_Library.h"
#include "Button_Pot_Init.h"
#include "Interrupt_Handlers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/***********************************
 *** Local function declarations ***
 **********************************/
void initialize();
void main_menu_interface();
void program_state();
void door();
void door_open();
void door_close();
void motor();
void lights();


/**********PIN MAP************
 * * * * * * * * * * * * * * *
 *********PWM PINS************
 *  TA0.2 RED on RGB    > P2.5
 *  TA0.3 GRN on RGB    > P2.6
 *  TA0.4 Blue on RGB   > P2.7
 *                           *
 *  TA2.2 DC Motor      > P5.7
 *  TA2.3 Servo         > P6.6
 *  TA2.4 LCD V0        > P6.7
 *                           *
 *********Keypad**************
 *-> GPIO                    *
 *  Row 1               > P3.0
 *  Row 2               > P3.2
 *  Row 3               > P3.3
 *  Row 4               > P3.5
 *  Col 1               > P1.5
 *  Col 2               > P1.6
 *  Col 3               > P1.7
 *                           *
 *********LCD*****************
 *-> GPIO                    *
 *  RS                  > P4.0
 *  E                   > P4.1
 *  DB4                 > P4.4
 *  DB5                 > P4.5
 *  DB6                 > P4.6
 *  DB7                 > P4.7
 *                           *
 *  ADC Pot Input       > P6.0
 *                           *
 ********Buttons**************
 *-> Set as Port Interrupts  *
 *                           *
 *  LED Toggle          > P2.3
 *  Motor Stop          > P5.5
 *
 ********Warning LED**********
 *-> Set as GPIO             *
 *                           *
 *  Yel LED             > P5.0
 *                           *
 ********Door LEDs************
 *-> Set as GPIO             *
 *                           *
 *  Green LED           > P3.6
 *  Red LED             > P3.7
 *                           *
 ****************************/

/**********************************
***** Char array for RGB text *****
**********************************/
char led_txt[3][6] = {"RED", "GREEN", "BLUE"};

/**********************************
***** State machine variables *****
**********************************/
enum State_enum {MAIN_MENU, DOOR, MOTOR, LIGHTS};
static unsigned int state;

/**********************************
************* main ****************
**********************************/
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

	initialize();                               //initialize peripherals

	/* Enable necessary interrupts */
    NVIC_EnableIRQ(ADC14_IRQn);
    NVIC_EnableIRQ(PORT2_IRQn);
    NVIC_EnableIRQ(PORT5_IRQn);
    NVIC_EnableIRQ(TA1_0_IRQn);
    NVIC_EnableIRQ(TA3_0_IRQn);
    __enable_irq();

    startup_display();                          //Display starting text on LCD
    ADC14->CTL0 |= ADC14_CTL0_SC;               //Start conversion of LCD potentiometer
	while (1){
	    program_state();                        //Enter user interface program
	}
}

/*****************************************************************************************
 * Function:    initialize()
 * Description: Runs all initialization/setup functions for timers and external
 *              components
*****************************************************************************************/
void initialize(){

    init_buttons();
    SysTickInit_NoInterrupts();
    init_TA0_Pins();
    init_TA1();
    init_TA2_Pins();
    TIMER_A3_delay();
    keypad_init();
    LCD_Pot();
    lcdInit();
}

/*****************************************************************************************
 * Function:    program_state()
 * Description: State machine to facilitate the program's primary functions.
 *              Each state reflects an interactive menu, and first calls a display
 *              function to inform the user of their options before receiving input.
 *              Each state then calls an interface function, which may consist of several
 *              subfunctions, gathering keypad input from the user and performing a task
 *              before returning to the main menu.
*****************************************************************************************/
void program_state(){

    switch(state){
        case MAIN_MENU:
            main_menu_display();
            main_menu_interface();
            break;
        case DOOR:
            door_menu_display();
            door();
            break;
        case MOTOR:
            motor_menu_display();
            motor();
            break;
        case LIGHTS:
            lights_menu_display();
            lights();
            break;
    }
}
/*****************************************************************************************
 * Function:    main_menu_interface()
 * Description: Read the keypad for user input. User will choose Door, Motor, or Lights
 *              Menu.
*****************************************************************************************/
void main_menu_interface(){

    int i;
    i = read_single_digit(1, 4, 0);     //accept user input 1 - 3
    if (i == 1)
        state = DOOR;
    if (i == 2)
        state = MOTOR;
    if (i == 3)
        state = LIGHTS;
    lcdClear();

}
/*****************************************************************************************
 * Function:    door()
 * Description: Interface for the door menu.
 *              Receives user keypad input to open or close the door with the assistance
 *              of subfunctions door_open() and door_close().
 *              Displays the user's choice on the LCD before returning to main menu.
*****************************************************************************************/
void door(){
    int door_state;
    door_state = read_single_digit(1, 3, 0);    //accept input 1 or 2
    if (door_state == 1){
        door_open();                            //Manipulate servo
        lcdClear();
        SysTick_delay_ms(100);
        lcdSetText("DOOR OPEN", 0, 0);
        SysTick_delay_ms(1500);
        lcdClear();
        state = MAIN_MENU;
    }
    if (door_state == 2){
        door_close();                           //Manipulate servo
        lcdClear();
        SysTick_delay_ms(100);
        lcdSetText("DOOR CLOSED", 0, 0);
        SysTick_delay_ms(1500);
        lcdClear();
        state = MAIN_MENU;
    }
}
/*****************************************************************************************
 * Function:    door_open()
 * Description: Adjusts servo PWM to open the door
 *              Turn Red indicator LED off and Green LED on
*****************************************************************************************/
void door_open(){
    TIMER_A2->CCR[3] = 3100;                //Set Servo PWM to open door
    SysTick_delay_ms(500);                  //delay to allow door to open
    TIMER_A2->CCR[3] = 0;                   //disable PWM
    P3->OUT &= ~BIT7;                       //turn off Red LED
    P3->OUT |= BIT6;                        //turn on Green LED
}

/*****************************************************************************************
 * Function:    door_close()
 * Description: Adjusts servo PWM to close the door
 *              Turn Green indicator LED off and Red LED on
*****************************************************************************************/
void door_close(){
    TIMER_A2->CCR[3] = 6000;                //Set Servo PWM to close door
    SysTick_delay_ms(500);                  //delay to allow door to close
    TIMER_A2->CCR[3] = 0;                   //disable PWM
    P3->OUT &= ~BIT6;                       //turn off Green LED
    P3->OUT |= BIT7;                        //turn on Red LED
}

/*****************************************************************************************
 * Function:    motor()
 * Description: Interface for the door menu.
 *              Receives user keypad input to set the speed of the DC motor.
 *              Displays the user's choice on the LCD before returning to main menu.
 *              If duty cycle exceeds 80%, initiate a TA0 interrupt to toggle a blinking
 *              warning LED.
*****************************************************************************************/
void motor(){
    float motor_set;
    motor_set = read_single_digit(0, 10, 1);        //Accept input 0 - 9
    if(motor_set>=0){
        lcdSetText("SPEED", 0, 0);                  //Write user's choice to the LCD
        lcdSetInt(motor_set, 0, 1);
        SysTick_delay_ms(500);
        TIMER_A2->CCR[2] = (motor_set / 10) * 60000;//update DC motor duty cycle
        if (TIMER_A2->CCR[2] >= 48000)              //duty cycle >= 80%
            TIMER_A1->CTL |= (BIT4 | TIMER_A_CTL_CLR);  //initiate TA0 interrupt
        else TIMER_A1->CTL &= ~BIT4;                //duty cycle < 80%, disable interrupt
        SysTick_delay_ms(1500);
        lcdClear();
        state = MAIN_MENU;
    }
}
/*****************************************************************************************
 * Function:    lights()
 * Description: Interface for the lights menu.
 *              Receives user keypad input to choose the Red, Green, or Blue LED on the
 *              RGB module.
 *              Choice is stored as a variable that reflects which TA0->CCRn instance to
 *              update. Receives user input 0 - 100 to set the brightness level of the
 *              chosen LED. New duty cycle calculated and assigned to the proper CCRn register.
 *              Duty cycles are global variables, allowing the CCRn registers to be
 *              toggled on and off with a button press.
 *              Displays the user's choice on the LCD before returning to main menu.
*****************************************************************************************/
void lights(){
    int i, j;
    float level;

    i = read_single_digit(1, 4, 0);                 //accept input 1 - 3
    lcdClear();
    for(j = 0; j < strlen(led_txt[i-1]); j++){      //write user's LED choice to the LCD
        lcdSetChar(led_txt[i-1][j], j, 0);
    }
    lcdSetText("SET LVL 0-100", 0, 1);              //Display instructions
    lcdSetText("[*] TO CLR ENTRY", 0, 2);
    SysTick_delay_ms(500);
    level = RGB_brightness();                       //read user entry
    RGB_DC[i] = (level / 100) * 3000;               //calculate new duty cycle
    if (RGB_DC[i] > 0) LED_check = 1;               //set flag for LED button
    TIMER_A0->CCR[i + 1] = RGB_DC[i];               //input duty cycle to CCR[i + 1] (starts on CCR2)
    lcdClear();
    state = MAIN_MENU;
}

