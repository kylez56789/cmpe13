// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Ascii.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"
#include "Buttons.h"

// **** Set macros and preprocessor directives ****
#define window 1.5
#define leftmost 0x80
#define rightmost 0x01
#define LEFT 0
#define RIGHT 1
#define base 5
#define maxtime 20
#define initial 0x01
#define bitmask1 0x03
#define bitmask2 0xC
#define bitmask3 0x30
#define bitmask4 0xC0
// **** Declare any datatypes here ****

struct AdcResult {
    uint8_t event;
    int16_t voltage;
};

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};
// **** Define global, module-level, or external variables here ****
static struct Timer timer;
static int direction;
unsigned char led;
static uint8_t switchesState;
static struct AdcResult adc;
double previous, current;
char meter[50];
static int volt;
static double pct;
uint8_t buttonEvents;
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


    // Enable interrupts for the ADC
    IPC6bits.AD1IP = 2;
    IPC6bits.AD1IS = 0;
    IEC1bits.AD1IE = 1;

    // Set B2 to an input so AN2 can be used by the ADC.
    TRISBbits.TRISB2 = 1;

    // Configure and start the ADC
    AD1CHSbits.CH0SA = 2; // add B2 to the mux
    AD1PCFGbits.PCFG2 = 0; // add b2 to the ADC
    AD1CSSLbits.CSSL2 = 1; // and add b2 to the scanner

    AD1CON1 = 0; // start with 0
    AD1CON1bits.SSRC = 0b111; // auto conversion
    AD1CON1bits.ASAM = 1; // auto sample

    AD1CON2 = 0; // start with 0
    AD1CON2bits.SMPI = 7; // one interrupt per 8 samples

    AD1CON3 = 0; // start with 0
    AD1CON3bits.SAMC = 29; // long sample time
    AD1CON3bits.ADCS = 50; // long conversion time

    AD1CON1bits.ADON = 1; // and finally turn it on


    printf("Welcome to CRUZID's lab6 part6 (Extra Credit).  Compiled on %s %s.\n", __TIME__, __DATE__);
    //adc
    OledInit();
    ButtonsInit();
    LEDS_INIT();
    //switches
    timer.timeRemaining = (maxtime - (int) pct / base) + 1;
    LEDS_SET(initial);
    direction = LEFT;
    while (1) {

        if (adc.event == TRUE) {
            volt = adc.voltage;
            pct = ((double) volt / 1023) * 100;
            sprintf(meter, "Potentiometer value: \n%4d\n%3.0f%%", adc.voltage, pct);
            OledDrawString(meter);
            OledUpdate();
            adc.event = FALSE;
        }

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

//        if (buttonEvents) {
//            if (switchesState & SWITCH_STATE_SW1 && buttonEvents & BUTTON_EVENT_1UP) {
//                ledstate = LEDS_GET();
//                ledstate ^= bitmask1;
//                LEDS_SET(ledstate);
//            } else if (!(switchesState & SWITCH_STATE_SW1) && buttonEvents & BUTTON_EVENT_1DOWN) {
//                ledstate = LEDS_GET();
//                ledstate ^= bitmask1;
//                LEDS_SET(ledstate);
//            }
//            if (switchesState & SWITCH_STATE_SW2 && buttonEvents & BUTTON_EVENT_2UP) {
//                ledstate = LEDS_GET();
//                ledstate ^= bitmask2;
//                LEDS_SET(ledstate);
//            } else if (!(switchesState & SWITCH_STATE_SW2) && buttonEvents & BUTTON_EVENT_2DOWN) {
//                ledstate = LEDS_GET();
//                ledstate ^= bitmask2;
//                LEDS_SET(ledstate);
//            }
//            if (switchesState & SWITCH_STATE_SW3 && buttonEvents & BUTTON_EVENT_3UP) {
//                ledstate = LEDS_GET();
//                ledstate ^= bitmask3;
//                LEDS_SET(ledstate);
//            } else if (!(switchesState & SWITCH_STATE_SW3) && buttonEvents & BUTTON_EVENT_3DOWN) {
//                ledstate = LEDS_GET();
//                ledstate ^= bitmask3;
//                LEDS_SET(ledstate);
//            }
//            if (switchesState & SWITCH_STATE_SW4 && buttonEvents & BUTTON_EVENT_4UP) {
//                ledstate = LEDS_GET();
//                ledstate ^= bitmask4;
//                LEDS_SET(ledstate);
//            } else if (!(switchesState & SWITCH_STATE_SW4) && buttonEvents & BUTTON_EVENT_4DOWN) {
//                ledstate = LEDS_GET();
//                ledstate ^= bitmask4;
//                LEDS_SET(ledstate);
//            }
//            buttonEvents = 0;
//        }
    }
    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    timer.timeRemaining -= 1;
    if (timer.timeRemaining == 0) {
        timer.event = TRUE;
        timer.timeRemaining = (maxtime - (int) pct / base) + 1;
    }

    buttonEvents = ButtonsCheckEvents();
    switchesState = SWITCH_STATES();

}

/**
 * This is the ISR for the ADC1 peripheral.
 */
void __ISR(_ADC_VECTOR, IPL2AUTO) AdcHandler(void)
{
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;

    current = ADC1BUF0;
    current += ADC1BUF1;
    current += ADC1BUF2;
    current += ADC1BUF3;
    current += ADC1BUF4;
    current += ADC1BUF5;
    current += ADC1BUF6;
    current += ADC1BUF7;
    current /= 8;
    if (current == 0) {
        adc.event = TRUE;
        adc.voltage = 0;
        previous = current;
    }
    if (current == 1023) {
        adc.event = TRUE;
        adc.voltage = 1023;
        previous = current;
    }
    if (current - previous >= window || current - previous <= -window) {
        adc.event = TRUE;
        adc.voltage = current;
        previous = current;
    }

}