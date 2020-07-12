// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>
#include "Oled.h"
#include "OledDriver.h"
#include "Ascii.h"

// User libraries


// **** Set macros and preprocessor directives ****
#define window 1.5
// **** Declare any datatypes here ****

struct AdcResult {
    uint8_t event;
    int16_t voltage;
};

// **** Define global, module-level, or external variables here ****
static struct AdcResult adc;
double previous, current;
// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

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

    printf("Welcome to kmzhang's lab6 part3 (bounce_adc).  Compiled on %s %s.\n", __TIME__, __DATE__);
    OledInit();
    char meter[50];
    int volt;
    double pct;
    while (1) {
        if (adc.event == TRUE) {
            volt = adc.voltage;
            pct = ((double)volt / 1023) * 100;
            sprintf(meter, "Potentiometer value: \n%4d\n%3.0f%%", adc.voltage, pct);
            OledDrawString(meter);
            OledUpdate();
            adc.event = FALSE;
        }
    }
    while (1);
}

/**
 * This is the interrupt for the ADC1 peripheral. It will trigger whenever a new ADC reading is available
 * in the ADC buffer SFRs, ADC1BUF0 - ADC1BUF7.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_ADC_VECTOR, ipl2auto) AdcHandler(void)
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
    if (current == 0){
        adc.event = TRUE;
        adc.voltage = 0;
        previous = current;
    }
    if (current == 1023){
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