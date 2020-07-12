// **** Include libraries here ****
// Standard C libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Ascii.h"
#include "Oled.h"
#include "OledDriver.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
//------don't include BinaryTree.h or Morse.h!-----
#include "Morse.h"

#define ASCII_WIDTH_MAX 256
#define SPACE ' '
#define RESET '0'
//Helper functions:
void OledAddToTopLine(MorseEvent event);
void OledClearTopLine(MorseEvent event);
void OledAddToBottomLine(MorseEvent event);
static MorseEvent morseEvent;
static MorseEvent decodeEvent;
static int x1 = 0;
static int y1 = 0;
static int x2 = 0;
static int y2 = ASCII_FONT_HEIGHT;
static int fatal;

int main()
{
    BOARD_Init();

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

    printf("\nWelcome to kmzhang's Lab8 Morse decoder!  Compiled on %s %s\n", __DATE__, __TIME__);

    //initialization code
    fatal = MorseInit();
    if (fatal == 0){
        FATAL_ERROR();
    }
    OledInit();
    while (1) {
        if (morseEvent.type) {
            if (morseEvent.type == MORSE_EVENT_DOT || morseEvent.type == MORSE_EVENT_DASH) {
                OledAddToTopLine(morseEvent);
                decodeEvent = MorseDecode(morseEvent);
                if (decodeEvent.type == MORSE_EVENT_ERROR) {
                    OledClearTopLine(morseEvent);
                    OledAddToBottomLine(decodeEvent);
                }
            } else if (morseEvent.type == MORSE_EVENT_NEW_LETTER) {
                decodeEvent.parameter = RESET;
                decodeEvent = MorseDecode(morseEvent);
                OledAddToBottomLine(decodeEvent);
                OledClearTopLine(morseEvent);
            } else if (morseEvent.type == MORSE_EVENT_NEW_WORD) {
                OledAddToBottomLine(morseEvent);
                OledClearTopLine(morseEvent);
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

    IFS0CLR = 1 << 8;
    morseEvent = MorseCheckEvents();
}

void OledAddToTopLine(MorseEvent event)
{
    if (event.type == MORSE_EVENT_DOT) {
        OledDrawChar(x1, y1, MORSE_CHAR_DOT);
        OledUpdate();
        x1 += ASCII_FONT_WIDTH;
    } else if (event.type == MORSE_EVENT_DASH) {
        OledDrawChar(x1, y1, MORSE_CHAR_DASH);
        OledUpdate();
        x1 += ASCII_FONT_WIDTH;
    }
}

void OledClearTopLine(MorseEvent event)
{
    for (x1 = 0; x1 < ASCII_WIDTH_MAX; x1 += ASCII_FONT_WIDTH) {
        OledDrawChar(x1, y1, SPACE);
        OledUpdate();
    }
    x1 = 0;
}

void OledAddToBottomLine(MorseEvent event)
{
    if (event.type == MORSE_EVENT_NEW_WORD) {
        OledDrawChar(x2, y2, SPACE);
        OledUpdate();
        x2 += ASCII_FONT_WIDTH;
        event.type = MORSE_EVENT_NONE;
    } else if (event.type == MORSE_EVENT_CHAR_DECODED) {
        OledDrawChar(x2, y2, decodeEvent.parameter);
        OledUpdate();
        x2 += ASCII_FONT_WIDTH;
        event.type = MORSE_EVENT_NONE;
    } else if (event.type == MORSE_EVENT_ERROR && decodeEvent.parameter == MORSE_CHAR_BAD_CHAR) {
        OledDrawChar(x2, y2, MORSE_CHAR_BAD_CHAR);
        OledUpdate();
        x2 += ASCII_FONT_WIDTH;
        event.type = MORSE_EVENT_NONE;
    }
}
