// **** Include libraries here ****
// Standard C libraries


//CMPE13 Support Library
#include "BOARD.h"
#include <stdio.h>
// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
//------don't include BinaryTree.h or Morse.h!-----
#include "Morse.h"
#define RESET '0'
static MorseEvent morseEvent;
static MorseEvent decodeEvent;
static int init;

int main()
{
    BOARD_Init();


    printf("Welcome to CRUZID's Lab8 Morse.h tester!  Compiled on %s %s\n", __DATE__, __TIME__);

    printf("Beginning automatic portion of Morse.h tester:\n");
    printf("Testing MorseInit()\n");
    init = MorseInit();
    if (init != 0) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("Testing MorseDecode()\n");
    decodeEvent.type = MORSE_EVENT_DOT;
    decodeEvent = MorseDecode(decodeEvent);
    if (decodeEvent.type == MORSE_EVENT_NONE) {
        printf("PASSED TEST1\n");
    } else {
        printf("FAILED TEST1\n");
    }
    decodeEvent.type = MORSE_EVENT_NEW_LETTER;
    decodeEvent = MorseDecode(decodeEvent);
    if (decodeEvent.parameter == 'E') {
        printf("PASSED TEST2\n");
    } else {
        printf("FAILED TEST2\n");
    }
    decodeEvent.type = MORSE_EVENT_DOT;
    MorseDecode(decodeEvent);
    MorseDecode(decodeEvent);
    decodeEvent.type = MORSE_EVENT_DASH;
    MorseDecode(decodeEvent);
    MorseDecode(decodeEvent);
    decodeEvent.type = MORSE_EVENT_NEW_LETTER;
    decodeEvent = MorseDecode(decodeEvent);
    if (decodeEvent.type == MORSE_EVENT_ERROR) {
        printf("PASSED TEST3\n");
    } else {
        printf("FAILED TEST3\n");
    }

    printf("Beginning interactive portion of Morse.h tester:\n");

    // <editor-fold defaultstate="collapsed" desc="Configure Timer">


    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // </editor-fold>

    while (1) {
        if (morseEvent.type) {
            if (morseEvent.type == MORSE_EVENT_DOT) {
                decodeEvent = MorseDecode(morseEvent);
                printf(".");
                if (decodeEvent.type == MORSE_EVENT_ERROR) {
                    printf("%c", decodeEvent.parameter);
                }
            } else if (morseEvent.type == MORSE_EVENT_DASH) {
                decodeEvent = MorseDecode(morseEvent);
                printf("-");
            } else if (morseEvent.type == MORSE_EVENT_NEW_LETTER) {
                decodeEvent.parameter = RESET;
                decodeEvent = MorseDecode(morseEvent);
                if (decodeEvent.parameter != NULL) {
                    printf("%c", decodeEvent.parameter);
                }
            } else {
                printf(" ");
            }
            morseEvent.type = MORSE_EVENT_NONE;
        }
    };
}

/* Timer2 is used to check for Morse events.
 * 
 * You shouldn't need to modify this ISR!
 */
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    morseEvent = MorseCheckEvents();
}

