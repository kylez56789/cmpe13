// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Buttons.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Buttons.h"
// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****
uint8_t buttonEvents;
// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:64 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on
    printf("Welcome to CRUZID's lab6 part4 (ButtonsTest).  Compiled on %s %s.\n", __TIME__, __DATE__);

    printf("Please press some buttons!\n");
    ButtonsInit();
    while (1) {
        if (buttonEvents) {
            printf("buttonEvents = %X\n", buttonEvents);
            printf("EVENT:  4:");
            if (buttonEvents & BUTTON_EVENT_4UP) {
                printf("  UP ");
            } else if (buttonEvents & BUTTON_EVENT_4DOWN) {
                printf("DOWN ");
            } else {
                printf("---- ");
            }
            printf("3:");
            if (buttonEvents & BUTTON_EVENT_3UP) {
                printf("  UP ");
            } else if (buttonEvents & BUTTON_EVENT_3DOWN) {
                printf("DOWN ");
            } else {
                printf("---- ");
            }
            printf("2:");
            if (buttonEvents & BUTTON_EVENT_2UP) {
                printf("  UP ");
            } else if (buttonEvents & BUTTON_EVENT_2DOWN) {
                printf("DOWN ");
            } else {
                printf("---- ");
            }
            printf("1:");
            if (buttonEvents & BUTTON_EVENT_1UP) {
                printf("  UP \n");
            } else if (buttonEvents & BUTTON_EVENT_1DOWN) {
                printf("DOWN \n");
            } else {
                printf("---- \n");
            }
            buttonEvents = 0;
        }
    }
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    buttonEvents = ButtonsCheckEvents();
}