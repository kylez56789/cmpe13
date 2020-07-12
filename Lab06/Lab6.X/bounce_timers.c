// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "LEDS_Lab06.h"

#define timeA 76
#define timeB 114
#define timeC 190
#define timeAflag 0x01
#define timeBflag 0x02
#define timeCflag 0x04


// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****
static struct Timer TimerA;
static struct Timer TimerB;
static struct Timer TimerC;
char ledstate;

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

    printf("Welcome to kmzhang's lab6 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);

    TimerA.timeRemaining = timeA;
    TimerB.timeRemaining = timeB;
    TimerC.timeRemaining = timeC;
    LEDS_INIT();

    while (1) {
        if (TimerA.event == TRUE) {
            TimerA.event = FALSE;
            printf("A");
            ledstate = LEDS_GET();
            ledstate ^= timeAflag;
            LEDS_SET(ledstate);
        }
        if (TimerB.event == TRUE) {
            TimerB.event = FALSE;
            printf("B");
            ledstate = LEDS_GET();
            ledstate ^= timeBflag;
            LEDS_SET(ledstate);

        }
        if (TimerC.event == TRUE) {
            TimerC.event = FALSE;
            printf("C");
            ledstate = LEDS_GET();
            ledstate ^= timeCflag;
            LEDS_SET(ledstate);
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

    TimerA.timeRemaining -= 1;
    TimerB.timeRemaining -= 1;
    TimerC.timeRemaining -= 1;
    if (TimerA.timeRemaining == 0) {
        TimerA.event = TRUE;
        TimerA.timeRemaining = timeA;
    }
    if (TimerB.timeRemaining == 0) {
        TimerB.event = TRUE;
        TimerB.timeRemaining = timeB;
    }
    if (TimerC.timeRemaining == 0) {
        TimerC.event = TRUE;
        TimerC.timeRemaining = timeC;
    }
}