
// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>



//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>
#include "Leds_Lab06.h"
// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****
static struct Timer timer;
static int direction;
unsigned char led;
static uint8_t switchesState;
#define leftmost 0x80
#define rightmost 0x01
#define LEFT 0
#define RIGHT 1
#define base 3
#define initial 0x01

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

    printf("Welcome to kmzhang's lab6 part2 (bounce_switch).  Compiled on %s %s.\n", __TIME__, __DATE__);
    switchesState = SWITCH_STATES();
    timer.timeRemaining = (switchesState + 1) * base;
    LEDS_INIT();
    LEDS_SET(initial);
    direction = LEFT;
    while (1) {
        uint8_t currled = LEDS_GET();
        if (timer.event == TRUE) {
            timer.event = FALSE;
            if (direction == LEFT && currled == leftmost) {
                direction = RIGHT;
                led = LEDS_GET();
                led = led >> 1;
                LEDS_SET(led);
            } else if (direction == RIGHT && currled == rightmost) {
                led = LEDS_GET();
                direction = LEFT;
                led = led << 1;
                LEDS_SET(led);
            } else if (direction == LEFT) {
                led = LEDS_GET();
                led = led << 1;
                LEDS_SET(led);
            } else {
                led = LEDS_GET();
                led = led >> 1;
                LEDS_SET(led);
            }
        }
    }
}

/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;
    switchesState = SWITCH_STATES();
    timer.timeRemaining -= 1;
    if (timer.timeRemaining == 0) {
        timer.event = TRUE;
        timer.timeRemaining = (switchesState + 1) * base;
    }
}