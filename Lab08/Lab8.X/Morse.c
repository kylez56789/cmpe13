/*
 * File:   Morse.c
 * Author: kylez
 *
 * Created on May 23, 2019, 9:39 PM
 */


#include "xc.h"
#include "BOARD.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "BinaryTree.h"
#include "Morse.h"
#include "Buttons.h"
#define MORSE "#EISH54V#3UF####2ARL#####WP##J#1TNDB6#X##KC##Y##MGZ7#Q##0#8##90"

typedef enum {
    DOT,
    DASH,
    WAITING_FOR_WORD,
    WAITING_FOR_LETTER,
    WAITING_FOR_PULSE
} MorseState;

static MorseState state = WAITING_FOR_WORD;
static Node *MorseRoot;
static Node *currentNode;
static uint8_t buttonEvents;
static MorseEvent event;
static uint8_t timerdot = 0;
static uint8_t timerletter = 0;
static uint8_t timerword = 0;

int MorseInit(void)
{
    ButtonsInit();
    MorseRoot = TreeCreate(6, MORSE);
    currentNode = MorseRoot;
    if (MorseRoot != NULL) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

MorseEvent MorseDecode(MorseEvent input_event)
{
    if (input_event.type == MORSE_EVENT_DOT) {
        currentNode = GetLeftChild(currentNode);
        if (currentNode == NULL) {
            input_event.type = MORSE_EVENT_ERROR;
            input_event.parameter = MORSE_CHAR_BAD_CHAR;
            currentNode = MorseRoot;
            return input_event;
        } else {
            input_event.type = MORSE_EVENT_NONE;
            return input_event;
        }
    }
    if (input_event.type == MORSE_EVENT_DASH) {
        currentNode = GetRightChild(currentNode);
        if (currentNode == NULL) {
            input_event.type = MORSE_EVENT_ERROR;
            currentNode = MorseRoot;
            input_event.parameter = MORSE_CHAR_BAD_CHAR;
            return input_event;
        } else {
            input_event.type = MORSE_EVENT_NONE;
            return input_event;
        }
    } else if (input_event.type == MORSE_EVENT_NEW_LETTER && currentNode != MorseRoot) {
        if (currentNode->data == MORSE_CHAR_BAD_CHAR) {
            input_event.type = MORSE_EVENT_ERROR;
            input_event.parameter = MORSE_CHAR_BAD_CHAR;
            currentNode = MorseRoot;
            return input_event;
        } else {
            input_event.type = MORSE_EVENT_CHAR_DECODED;
            input_event.parameter = currentNode->data;
            currentNode = MorseRoot;
            return input_event;
        }
    }
    return input_event;
}

MorseEvent MorseCheckEvents(void)
{
    buttonEvents = ButtonsCheckEvents();
    switch (state) {
    case(WAITING_FOR_WORD):
        if (buttonEvents & BUTTON_EVENT_4DOWN) {
            state = DOT;
            timerdot = MORSE_DOT_TIMEOUT;
        }
        event.type = MORSE_EVENT_NONE;
        return event;
        break;
    case(DOT):
        timerdot -= 1;
        if (timerdot <= 0) {
            timerdot = MORSE_DOT_TIMEOUT;
            state = DASH;
        } else if (buttonEvents & BUTTON_EVENT_4UP) {
            state = WAITING_FOR_PULSE;
            timerword = MORSE_WORD_TIMEOUT;
            timerletter = MORSE_LETTER_TIMEOUT;
            event.type = MORSE_EVENT_DOT;
            return event;
        }
        break;
        ;
    case(DASH):
        if (buttonEvents & BUTTON_EVENT_4UP) {
            state = WAITING_FOR_PULSE;
            timerword = MORSE_WORD_TIMEOUT;
            timerletter = MORSE_LETTER_TIMEOUT;
            event.type = MORSE_EVENT_DASH;
            return event;
        }
        break;
    case(WAITING_FOR_PULSE):
        timerletter -= 1;
        timerword -= 1;
        if (timerletter <= 0) {
            state = WAITING_FOR_LETTER;
            event.type = MORSE_EVENT_NEW_LETTER;
            return event;
        } else if (timerword <= 0) {
            state = WAITING_FOR_WORD;
            event.type = MORSE_EVENT_NEW_WORD;
            return event;
        } else if (buttonEvents & BUTTON_EVENT_4DOWN) {
            state = DOT;
            timerdot = MORSE_DOT_TIMEOUT;
        }
        event.type = MORSE_EVENT_NONE;
        return event;
        break;
    case(WAITING_FOR_LETTER):
        timerword -= 1;
        if (timerword <= 0) {
            state = WAITING_FOR_WORD;
            event.type = MORSE_EVENT_NEW_WORD;
            return event;
        } else if (buttonEvents & BUTTON_EVENT_4DOWN) {
            state = DOT;
            timerdot = MORSE_DOT_TIMEOUT;
        }
        event.type = MORSE_EVENT_NONE;
        return event;
        break;
    }
    return event;
}



