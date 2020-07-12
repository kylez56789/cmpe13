// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Buttons.h"
#include "Leds_Lab06.h"
// **** Set macros and preprocessor directives ****
#define bitmask1 0x03
#define bitmask2 0xC
#define bitmask3 0x30
#define bitmask4 0xC0

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****
uint8_t buttonEvents;
static uint8_t switchesState;
uint8_t ledstate;
// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    printf("Welcome to CRUZID's lab6 part5 (bounce_buttons).  Compiled on %s %s.\n", __TIME__, __DATE__);
    ButtonsInit();
    LEDS_INIT();
    while (1) {
        if (buttonEvents) {
            if (switchesState & SWITCH_STATE_SW1 && buttonEvents & BUTTON_EVENT_1UP) {
                ledstate = LEDS_GET();
                ledstate ^= bitmask1;
                LEDS_SET(ledstate);
            } else if (!(switchesState & SWITCH_STATE_SW1) && buttonEvents & BUTTON_EVENT_1DOWN) {
                ledstate = LEDS_GET();
                ledstate ^= bitmask1;
                LEDS_SET(ledstate);
            }
            if (switchesState & SWITCH_STATE_SW2 && buttonEvents & BUTTON_EVENT_2UP) {
                ledstate = LEDS_GET();
                ledstate ^= bitmask2;
                LEDS_SET(ledstate);
            } else if (!(switchesState & SWITCH_STATE_SW2) && buttonEvents & BUTTON_EVENT_2DOWN) {
                ledstate = LEDS_GET();
                ledstate ^= bitmask2;
                LEDS_SET(ledstate);
            }
            if (switchesState & SWITCH_STATE_SW3 && buttonEvents & BUTTON_EVENT_3UP) {
                ledstate = LEDS_GET();
                ledstate ^= bitmask3;
                LEDS_SET(ledstate);
            } else if (!(switchesState & SWITCH_STATE_SW3) && buttonEvents & BUTTON_EVENT_3DOWN) {
                ledstate = LEDS_GET();
                ledstate ^= bitmask3;
                LEDS_SET(ledstate);
            }
            if (switchesState & SWITCH_STATE_SW4 && buttonEvents & BUTTON_EVENT_4UP) {
                ledstate = LEDS_GET();
                ledstate ^= bitmask4;
                LEDS_SET(ledstate);
            } else if (!(switchesState & SWITCH_STATE_SW4) && buttonEvents & BUTTON_EVENT_4DOWN) {
                ledstate = LEDS_GET();
                ledstate ^= bitmask4;
                LEDS_SET(ledstate);
            }
            buttonEvents = 0;
        }
    }
    while (1);
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
    switchesState = SWITCH_STATES();

}