/*
 * File:   Message.c
 * Author: kylez
 *
 * Created on May 31, 2019, 7:00 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Message.h"
#include "BOARD.h"
#include "xc.h"

#define CHA "CHA"
#define ACC "ACC"
#define REV "REV"
#define SHO "SHO"
#define RES "RES"
#define CHECKSUM_MAX_LEN 2

typedef enum {
    WAITING_FOR_START_DELIMITER,
    RECORDING_PAYLOAD,
    RECORDING_CHECKSUM
} decodestate;

static uint8_t payloadmax = 0;
static char load[MESSAGE_MAX_LEN];
static char chksum[CHECKSUM_MAX_LEN];
static uint8_t msgindex = 0;
static uint8_t checksumlen;
static decodestate state;

uint8_t Message_CalculateChecksum(const char* payload)
{
    uint8_t checksum = 0;
    const char* payld = payload;
    if (*payload == '$') {
        payload = payload + 1;
    }
    for (; *payload != '*' && *payload != NULL; payload++) {
        checksum ^= *payload;
    }
    payload = payld;
    return checksum;
}

/**
 * Given a payload and its checksum, verify that the payload matches the checksum
 * 
 * @param payload       //the payload of a message
 * @param checksum      //the checksum (in string form) of  a message,
 *                          should be exactly 2 chars long, plus a null char
 * @param message_event //An event corresponding to the parsed message.
 *                      //If the message could be parsed successfully,
 *                          message_event's type will correspond to the message type and 
 *                          its parameters will match the message's data fields.
 *                      //If the message could not be parsed,
 *                          message_events type will be BB_EVENT_ERROR
 * 
 * @return STANDARD_ERROR if:
 *              the payload does not match the checksum
 *              the checksum string is not two characters long
 *              the message does not match any message template
 *          SUCCESS otherwise
 * 
 * Please note!  sscanf() has a couple compiler bugs that make it a very
 * unreliable tool for implementing this function. * 
 */
int Message_ParseMessage(const char* payload,
        const char* checksum_string, BB_Event * message_event)
{
    int x = 0;
    uint16_t value;
    uint8_t checksum;
    const char* payload1 = payload;
    char checksumstr[20], msg1[20], msg2[20], msgtemp[20], *msgtype, *tok, payloadtemp[20] = {};
    checksum = Message_CalculateChecksum(payload);
    if (strlen(checksum_string) != 2) {
        return STANDARD_ERROR;
    }
    sprintf(checksumstr, "%02X", checksum);
    if (strcmp(checksumstr, checksum_string) != 0) {
        return STANDARD_ERROR;
    }
    for (x = 0; *payload != NULL; x++, payload++) {
        payloadtemp[x] = *payload;
    }
    payload = payload1;
    msgtype = strtok(payloadtemp, ",");
    tok = strtok(NULL, ",");
    if (tok == NULL) {
        return STANDARD_ERROR;
    } else {
        value = atof(tok);
        message_event->param0 = value;
    }
    tok = strtok(NULL, ",");
    if (tok != NULL) {
        message_event->param1 = atof(tok);
    }
    tok = strtok(NULL, ",");
    if (tok != NULL) {
        message_event->param2 = atof(tok);
    }
    if (strcmp(msgtype, CHA) == 0) {
        sprintf(msgtemp, PAYLOAD_TEMPLATE_CHA, message_event->param0);
        sprintf(msg1, MESSAGE_TEMPLATE, msgtemp, checksum);
        sprintf(msg2, MESSAGE_TEMPLATE, payload, checksum);
        if (strcmp(msg1, msg2) != 0) {
            return STANDARD_ERROR;
        } else {
            message_event->type = BB_EVENT_CHA_RECEIVED;
            return SUCCESS;
        }
    } else if (strcmp(msgtype, ACC) == 0) {
        sprintf(msgtemp, PAYLOAD_TEMPLATE_ACC, message_event->param0);
        sprintf(msg1, MESSAGE_TEMPLATE, msgtemp, checksum);
        sprintf(msg2, MESSAGE_TEMPLATE, payload, checksum);
        if (strcmp(msg1, msg2) != 0) {
            return STANDARD_ERROR;
        } else {
            message_event->type = BB_EVENT_ACC_RECEIVED;
            return SUCCESS;
        }
    } else if (strcmp(msgtype, REV) == 0) {
        sprintf(msgtemp, PAYLOAD_TEMPLATE_REV, message_event->param0);
        sprintf(msg1, MESSAGE_TEMPLATE, msgtemp, checksum);
        sprintf(msg2, MESSAGE_TEMPLATE, payload, checksum);
        if (strcmp(msg1, msg2) != 0) {
            return STANDARD_ERROR;
        } else {
            message_event->type = BB_EVENT_REV_RECEIVED;
            return SUCCESS;
        }
    } else if (strcmp(msgtype, SHO) == 0) {
        sprintf(msgtemp, PAYLOAD_TEMPLATE_SHO, message_event->param0, message_event->param1);
        sprintf(msg1, MESSAGE_TEMPLATE, msgtemp, checksum);
        sprintf(msg2, MESSAGE_TEMPLATE, payload, checksum);
        if (strcmp(msg1, msg2) != 0) {
            return STANDARD_ERROR;
        } else {
            message_event->type = BB_EVENT_SHO_RECEIVED;
            return SUCCESS;
        }
    } else if (strcmp(msgtype, RES) == 0) {
        sprintf(msgtemp, PAYLOAD_TEMPLATE_RES, message_event->param0, message_event->param1,
                message_event->param2);
        sprintf(msg1, MESSAGE_TEMPLATE, msgtemp, checksum);
        sprintf(msg2, MESSAGE_TEMPLATE, payload, checksum);
        if (strcmp(msg1, msg2) != 0) {
            return STANDARD_ERROR;
        } else {
            message_event->type = BB_EVENT_RES_RECEIVED;
            return SUCCESS;
        }
    } else {
        return STANDARD_ERROR;
    }
    return SUCCESS;
}

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_COO,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. 
 * 
 * The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * 
 * @param message            The character array used for storing the output. 
 *                              Must be long enough to store the entire string,
 *                              see MESSAGE_MAX_LEN.
 * @param message_to_encode  A message to encode
 * @return                   The length of the string stored into 'message_string'.
                             Return 0 if message type is MESSAGE_NONE.
 */
int Message_Encode(char *message_string, Message message_to_encode)
{
    int x;
    uint8_t checksum;
    char payload[20];
    char message[20];
    if (message_to_encode.type == MESSAGE_CHA) {
        sprintf(payload, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
    } else if (message_to_encode.type == MESSAGE_ACC) {
        sprintf(payload, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
    } else if (message_to_encode.type == MESSAGE_REV) {
        sprintf(payload, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
    } else if (message_to_encode.type == MESSAGE_SHO) {
        sprintf(payload, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0,
                message_to_encode.param1);
    } else if (message_to_encode.type == MESSAGE_RES) {
        sprintf(payload, PAYLOAD_TEMPLATE_RES, message_to_encode.param0,
                message_to_encode.param1, message_to_encode.param2);
    }
    checksum = Message_CalculateChecksum(payload);
    sprintf(message, MESSAGE_TEMPLATE, payload, checksum);
    memset(message_string + 1, '\0', 14 * sizeof (char));
    for (x = 0; message[x] != NULL; x++, message_string++) {
        *message_string = message[x];
    }
    return x;
}

/**
 * Message_Decode reads one character at a time.  If it detects a full NMEA message,
 * it translates that message into a BB_Event struct, which can be passed to other 
 * services.
 * 
 * @param char_in - The next character in the NMEA0183 message to be decoded.
 * @param decoded_message - a pointer to a message struct, used to "return" a message
 *                          if char_in is the last character of a valid message, 
 *                              then decoded_message
 *                              should have the appropriate message type.
 *                              otherwise, it should have type NO_EVENT.
 * @return SUCCESS if no error was detected
 *         STANDARD_ERROR if an error was detected
 * 
 * note that ANY call to Message_Decode may modify decoded_message.
 */
int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event)
{
    int resultofparse;
    switch (state) {
    case WAITING_FOR_START_DELIMITER:
        if (char_in == '$') {
            state = RECORDING_PAYLOAD;
            payloadmax++;
            decoded_message_event->type = BB_EVENT_NO_EVENT;
            return SUCCESS;
        }
        break;
    case RECORDING_PAYLOAD:
        if (payloadmax > MESSAGE_MAX_PAYLOAD_LEN) {
            state = WAITING_FOR_START_DELIMITER;
            return STANDARD_ERROR;
        } else if (char_in == '$' || char_in == '\n') {
            state = WAITING_FOR_START_DELIMITER;
            return STANDARD_ERROR;
        } else if (char_in == '*') {
            state = RECORDING_CHECKSUM;
            decoded_message_event->type = BB_EVENT_NO_EVENT;
            return SUCCESS;
        } else {
            load[msgindex] = char_in;
            msgindex++;
            decoded_message_event->type = BB_EVENT_NO_EVENT;
            return SUCCESS;
        }
        break;
    case RECORDING_CHECKSUM:
        if (checksumlen > CHECKSUM_MAX_LEN) {
            state = WAITING_FOR_START_DELIMITER;
            return STANDARD_ERROR;
        } else if (char_in == '\n' && checksumlen < CHECKSUM_MAX_LEN) {
            state = WAITING_FOR_START_DELIMITER;
            return STANDARD_ERROR;
        } else if ((char_in >= 48 && char_in <= 57) || (char_in >= 65 && char_in <= 70)) {
            chksum[checksumlen] = char_in;
            checksumlen++;
            return SUCCESS;
        }
        if (char_in == '\n') {
            msgindex = 0;
            checksumlen = 0;
            resultofparse = Message_ParseMessage(&load[msgindex], chksum, decoded_message_event);
            memset(load, '\0', MESSAGE_MAX_LEN * sizeof (char));
            memset(chksum, '\0', CHECKSUM_MAX_LEN * sizeof (char));
            if (resultofparse == SUCCESS) {
                state = WAITING_FOR_START_DELIMITER;
                return SUCCESS;
            } else {
                state = WAITING_FOR_START_DELIMITER;
                return STANDARD_ERROR;
            }
        }
        break;
    }
    return SUCCESS;
}