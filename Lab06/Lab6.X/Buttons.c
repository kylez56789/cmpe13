/*
 * File:   Buttons.c
 * Author: kmzhang | Kyle Zhang
 *
 * Created on May 13, 2019, 11:34 PM
 */


#include "xc.h"
#include "Buttons.h"
#include <stdio.h>
#include <GenericTypeDefs.h>
#include "BOARD.h"

#define up1 0x1
#define dn1 0x2
#define up2 0x3
#define dn2 0x4
#define up3 0x5
#define dn3 0x6
#define up4 0x7
#define dn4 0x8


static int debounce = BUTTONS_DEBOUNCE_PERIOD;
static uint8_t prev = BUTTON_EVENT_NONE;
static uint8_t prevstate = BUTTON_EVENT_NONE;
uint8_t state;
static uint8_t b1prev = up1;
static uint8_t b2prev = up2;
static uint8_t b3prev = up3;
static uint8_t b4prev = up4;

void ButtonsInit(void)
{
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}

uint8_t ButtonsCheckEvents(void)
{
    state = BUTTON_STATES();
    uint8_t events = BUTTON_EVENT_NONE;
    if (debounce == 0) {
        if (prevstate != state) {
            if (state & BUTTON_STATE_1 && b1prev & up1) {
                events |= BUTTON_EVENT_1DOWN;
                b1prev = dn1;
            } else if (prev & BUTTON_EVENT_1DOWN && b1prev & dn1) {
                events |= BUTTON_EVENT_1UP;
                b1prev = up1;
            } else {
                events |= BUTTON_EVENT_NONE;
            }
            if (state & BUTTON_STATE_2 && b2prev & up2) {
                events |= BUTTON_EVENT_2DOWN;
                b2prev = dn2;
            } else if (prev & BUTTON_EVENT_2DOWN && b2prev & dn2) {
                events |= BUTTON_EVENT_2UP;
                b2prev = up2;
            } else {
                events |= BUTTON_EVENT_NONE;
            }
            if (state & BUTTON_STATE_3 && b3prev & up3) {
                events |= BUTTON_EVENT_3DOWN;
                b3prev = dn3;
            } else if (prev & BUTTON_EVENT_3DOWN && b3prev & dn3) {
                events |= BUTTON_EVENT_3UP;
                b3prev = up3;
            } else {
                events |= BUTTON_EVENT_NONE;
            }
            if (state & BUTTON_STATE_4 && b4prev & up1) {
                events |= BUTTON_EVENT_4DOWN;
                b4prev = dn4;
            } else if (prev & BUTTON_EVENT_4DOWN && b4prev & dn4) {
                events |= BUTTON_EVENT_4UP;
                b4prev = up4;
            } else {
                events |= BUTTON_EVENT_NONE;
            }
            prevstate = state;
            prev = events;
            debounce = BUTTONS_DEBOUNCE_PERIOD;
            return events;
        } else {
            debounce = BUTTONS_DEBOUNCE_PERIOD;
            return BUTTON_EVENT_NONE;
        }
    } else {
        debounce--;
        return BUTTON_EVENT_NONE;
    }
}
