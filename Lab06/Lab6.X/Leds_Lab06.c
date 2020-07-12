/*
 * File:   Leds_Lab06.c
 * Author: kylez
 *
 * Created on May 9, 2019, 11:03 PM
 */


#include "xc.h"
#include "LEDS_Lab06.h"
#include <stdio.h>
#include <GenericTypeDefs.h>
#include "BOARD.h"

void LEDS_INIT(void)
{
    TRISE = 0x00;
    LATE = 0x00;
}

void LEDS_SET(char newPattern)
{
    LATE = newPattern;
}

char LEDS_GET(void)
{
    unsigned char a;
    a = LATE;
    return a;
}