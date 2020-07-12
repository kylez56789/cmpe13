// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>
#include "Adc.h"
#include "Leds.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Buttons.h"
#include "Ascii.h"


// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)
#define MIN_TEMP 300 //degress
#define TEMP_DEFAULT 350
#define TIME_DEFAULT 1
#define TEMP_MAX 555
#define TIME_MAX 256
#define LONG_PRESS 5
#define WINDOW 1.5
#define LEDSCOOKING 0xFF
#define LEDSDONE 0x00
#define BROIL_TEMP 500
#define TOGGLE 0xFE
#define DEGREE 0xF8
#define TOP_ON 0x01
#define TOP_OFF 0x02
#define BOT_ON 0x03
#define BOT_OFF 0x04
#define CLEARINITIAL1 1
#define CLEARINITIAL2 21
// **** Set any local typedefs here ****

typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING, END_COOKING
} OvenState;

typedef enum {
    BAKE, TOAST, BROIL
} CookMode;

typedef enum {
    TIME, TEMP
} Selector;

typedef struct {
    OvenState state;
    uint16_t temp; //degrees
    CookMode mode;
    uint16_t starttime; //ticks
    Selector input;
    uint16_t presstime; //ticks
} OvenData;

// **** Declare any datatypes here ****

// **** Define any module-level, global, or external variables here ****
OvenData oven_data = {SETUP, TEMP_DEFAULT, BAKE, TIME_DEFAULT, TIME, 0};
static uint8_t buttonEvents;
static uint16_t freeruntime = 0;
static uint8_t TIMER_TICK = FALSE;
static uint8_t starttime;
static uint16_t endtime;
static uint16_t elapsedtime;
static uint8_t adcevent;
static uint8_t ticksperled;
static uint16_t ledtime;
static uint16_t cleartime1 = CLEARINITIAL1;
static uint16_t cleartime2 = CLEARINITIAL2;
static uint16_t current1, current2;
static uint16_t timertime;
static uint16_t clear1, clear2;
static uint8_t led;
static uint8_t ledtoggle = TOGGLE;
static uint16_t seconds;
static uint16_t minutes;
static unsigned char top = TOP_OFF;
static unsigned char bot = BOT_OFF;

// **** Put any helper functions here ****
void LED_COUNTDOWN(OvenData ovendata); //function that updates LEDS

/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData)
{
    char *mode[5];
    if (ovenData.mode == 2) {
        *mode = "Broil";
    } else if (ovenData.mode == 1) {
        *mode = "Toast";
    } else if (ovenData.mode == 0) {
        *mode = "Bake";
    }
    char bake1[100];
    char bake2[100];
    char toast[100];
    char broil[100];

    sprintf(bake1, "|%c%c%c%c%c|  Mode: %s\n|     | >Time:  %u:%02u\n|-----|  Temp: %d%cF\n"
            "|%c%c%c%c%c|\n", top, top, top, top, top, *mode, minutes,
            seconds, ovenData.temp, DEGREE, bot, bot, bot, bot, bot);
    sprintf(bake2, "|%c%c%c%c%c|  Mode: %s\n|     |  Time:  %u:%02u\n|-----| >Temp: %d%cF\n"
            "|%c%c%c%c%c|\n", top, top, top, top, top, *mode, minutes,
            seconds, ovenData.temp, DEGREE, bot, bot, bot, bot, bot);
    sprintf(toast, "|%c%c%c%c%c|  Mode: %s\n|     |  Time:  %u:%02u\n|-----|\n"
            "|%c%c%c%c%c|\n", top, top, top, top, top, *mode, minutes,
            seconds, bot, bot, bot, bot, bot);
    sprintf(broil, "|%c%c%c%c%c|  Mode: %s\n|     |  Time:  %u:%02u\n|-----|  Temp: %d%cF\n"
            "|%c%c%c%c%c|\n", top, top, top, top, top, *mode, minutes,
            seconds, BROIL_TEMP, DEGREE, bot, bot, bot, bot, bot);
    OledClear(OLED_COLOR_BLACK);
    if (oven_data.mode == BAKE) {
        if (oven_data.input == TIME) {
            OledDrawString(bake1);
            OledUpdate();
        } else if (oven_data.input == TEMP) {
            OledDrawString(bake2);
            OledUpdate();
        }
    }
    if (oven_data.mode == TOAST) {
        OledDrawString(toast);
        OledUpdate();
    }
    if (oven_data.mode == BROIL) {
        OledDrawString(broil);
        OledUpdate();
    }

}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void)
{
    switch (oven_data.state) {
    case SETUP:
        if (buttonEvents & BUTTON_EVENT_3DOWN) {
            TIMER_TICK = TRUE;
            starttime = freeruntime;
            oven_data.state = SELECTOR_CHANGE_PENDING;
        } else if (buttonEvents & BUTTON_EVENT_4DOWN) {
            TIMER_TICK = TRUE;
            timertime = (oven_data.starttime * 5);
            ticksperled = timertime / 8;
            ledtime = (oven_data.starttime * 5);
            ledtime -= ticksperled;
            LEDS_SET(LEDSCOOKING);
            if (oven_data.mode == BAKE) {
                top = TOP_ON;
                bot = BOT_ON;
            } else if (oven_data.mode == TOAST) {
                top = TOP_OFF;
                bot = BOT_ON;
            } else {
                top = TOP_ON;
                bot = BOT_OFF;
            }
            updateOvenOLED(oven_data);
            oven_data.state = COOKING;
        } else if (adcevent == TRUE) {
            if (oven_data.input == TIME) {
                current1 = AdcRead();
                oven_data.starttime = (current1 >> 2) + 1;
                seconds = oven_data.starttime;
                seconds = seconds % 60;
                minutes = oven_data.starttime / 60;
                updateOvenOLED(oven_data);
            } else if (oven_data.input == TEMP) {
                current2 = AdcRead();
                oven_data.temp = (current2 >> 2) + 300;
            }
            updateOvenOLED(oven_data);
        }

        break;
    case SELECTOR_CHANGE_PENDING:
        if (buttonEvents & BUTTON_EVENT_3UP) {
            TIMER_TICK = FALSE;
            endtime = freeruntime;
            elapsedtime = endtime - starttime;
            if (elapsedtime < LONG_PRESS) {
                if (oven_data.mode == BROIL) {
                    oven_data.mode = BAKE;
                } else if (oven_data.mode == BAKE) {
                    oven_data.mode = TOAST;
                    oven_data.input = TIME;
                } else if (oven_data.mode == TOAST) {
                    oven_data.mode = BROIL;
                    oven_data.input = TIME;
                }
            } else if (elapsedtime >= LONG_PRESS && oven_data.mode == BAKE) {
                if (oven_data.input == TEMP) {
                    oven_data.input = TIME;
                } else {
                    oven_data.input = TEMP;
                }
            }
            oven_data.state = SETUP;
            updateOvenOLED(oven_data);
        }
        break;
    case COOKING:
        if (buttonEvents & BUTTON_EVENT_4DOWN) {
            starttime = freeruntime;
            oven_data.state = RESET_PENDING;
        }
        if (buttonEvents && !(buttonEvents & BUTTON_EVENT_4DOWN)) {
        }
        if (timertime == 0) {
            LEDS_SET(LEDSDONE);
            freeruntime = 0;
            top = TOP_OFF;
            bot = BOT_OFF;
            oven_data.state = END_COOKING;
            ledtoggle = TOGGLE;
            seconds = oven_data.starttime;
            seconds = seconds % 60;
            minutes = oven_data.starttime / 60;
            OledSetDisplayInverted();
        } else if (TIMER_TICK) {
            if (timertime > 0) {
                timertime -= 1;
                LED_COUNTDOWN(oven_data);
            }
        }
        break;
    case RESET_PENDING:
        if (buttonEvents & BUTTON_EVENT_4UP) {
            endtime = freeruntime;
            elapsedtime = endtime - starttime;
            if (elapsedtime >= LONG_PRESS) {
                freeruntime = 0;
                TIMER_TICK = FALSE;
                LEDS_SET(LEDSDONE);
                top = TOP_OFF;
                bot = BOT_OFF;
                ledtoggle = TOGGLE;
                oven_data.state = SETUP;
                seconds = oven_data.starttime;
                seconds = seconds % 60;
                minutes = oven_data.starttime / 60;
                updateOvenOLED(oven_data);
            } else {
                oven_data.state = COOKING;
            }
        } else if (TIMER_TICK) {
            if (timertime > 0) {
                timertime -= 1;
                LED_COUNTDOWN(oven_data);
            }
        }
        break;
    case END_COOKING:
        if (buttonEvents & BUTTON_EVENT_4DOWN) {
            oven_data.state = SETUP;
            OledSetDisplayNormal();
            updateOvenOLED(oven_data);
            TIMER_TICK = FALSE;
        } else if (TIMER_TICK) {
            if (clear1 == TRUE) {
                OledSetDisplayNormal();
                clear1 = FALSE;
            }
            if (clear2 == TRUE) {
                OledSetDisplayInverted();
                clear2 = FALSE;
            }
        }
        break;
    }
}

int main()
{
    BOARD_Init();

    //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">

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

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>

    printf("Welcome to kmzhang's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    //initialize state machine (and anything else you need to init) here
    OledInit();
    LEDS_INIT();
    ButtonsInit();
    AdcInit();
    seconds = TIME_DEFAULT;
    updateOvenOLED(oven_data);
    while (1) {
        if (adcevent == TRUE) {
            runOvenSM();
            adcevent = FALSE;
        }
        if (buttonEvents) {
            runOvenSM();
        }
        if (TIMER_TICK == TRUE) {
            runOvenSM();
            TIMER_TICK = FALSE;
        }
    };
}

void LED_COUNTDOWN(OvenData ovendata)
{
    seconds = (timertime / 5);
    minutes = seconds / 60;
    seconds = (timertime / 5) % 60;
    updateOvenOLED(oven_data);
    if (timertime == ledtime) {

        ledtime -= ticksperled;
        led = LEDS_GET();
        led = led & ledtoggle;
        ledtoggle = ledtoggle << 1;
        LEDS_SET(led);
    }
}

/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.

    IFS0CLR = 1 << 12;

    freeruntime += 1;
    TIMER_TICK = TRUE;
}

/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    buttonEvents = ButtonsCheckEvents();

    if (AdcChanged() == TRUE) {
        adcevent = TRUE;
    } else {
        adcevent = FALSE;
    }

    cleartime1 -= 1;
    if (cleartime1 == 0) {
        clear1 = TRUE;
        cleartime1 = 20;
    }
    cleartime2 -= 1;
    if (cleartime2 == 0) {
        clear2 = TRUE;
        cleartime2 = 40;
    }
}