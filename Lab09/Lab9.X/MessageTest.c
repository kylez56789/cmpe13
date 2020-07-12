// Allen Cheung | alkhcheu@ucsc.edu

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// Battleboats Libraries:
#include "Message.h"
#include "Field.h"

char testMessage[] = "$ACC,4123*69\n";
char testMessage2[] = "$RES,4,8,1*55\n";
char testMessage3[] = "$REV,12345*5C\n";    
char testMessage4[] = "$SHO,3,3*54\n";
char *payload1 = "ACC,4123";
char *payload2 = "RES,4,8,1";
char *chksum1 = "69";
char *chksum2 = "55";
char message;
char *messageptr = &message;
BB_Event bbevent;
BB_Event * event_ptr = &bbevent;
static Message message_in;
uint8_t checksum;
int x = 0, y = 0;
char char_in;

int main(void)
{
    BOARD_Init();
    printf("\nWelcome to alkhcheu's Message test harness, compiled on %s %s\n", __DATE__, __TIME__);
    
    checksum = Message_CalculateChecksum("CAT");
    printf("%X", checksum);
    
    printf("\t[Testing Message_CalculateChecksum]");
    printf("\n\tTest 1: ");
    checksum = Message_CalculateChecksum(testMessage);
    if (checksum == 0x69) {
        printf("\n\t->PASSED | Result = %X", checksum);
    } else {
        printf("\n\t->FAILED | Result = %X", checksum);
    }
    checksum = Message_CalculateChecksum(testMessage3);
    printf("\n\tTest 2: ");
    if (checksum == 0x5C) {
        printf("\n\t->PASSED | Result = %X", checksum);
    } else {
        printf("\n\t->FAILED | Result = %X", checksum);
    }

    printf("\n\n\t[Testing Message_Encode]");
    printf("\n\tTest 1:");
    message_in.param0 = 4123;
    message_in.type = MESSAGE_ACC;
    x = Message_Encode(messageptr, message_in);
    if (strcmp(messageptr, testMessage) == 0) {
        printf("\n\t->PASSED | Result = %s", messageptr);
    } else {
        printf("\n\t->FAILED | Result = %s", messageptr);
    }
    printf("\tTest 2:");
    message_in.param0 = 4;
    message_in.param1 = 8;
    message_in.param2 = 1;
    message_in.type = MESSAGE_RES;
    x = Message_Encode(messageptr, message_in);
    if (strcmp(messageptr, testMessage2) == 0) {
        printf("\n\t->PASSED | Result = %s", messageptr);
    } else {
        printf("\n\t->FAILED | Result = %s", messageptr);
    }
    
    printf("\n\tTest 3:");
    message_in.param0 = 12345;
    message_in.type = MESSAGE_REV;
    x = Message_Encode(messageptr, message_in);
    if (strcmp(messageptr, testMessage3) == 0) {
        printf("\n\t->PASSED | Result = %s", messageptr);
    } else {
        printf("\n\t->FAILED | Result = %s", messageptr);
    }

    printf("\n\t[Testing Message_ParseMessage]");

    printf("\n\tTest 1:");
    x = Message_ParseMessage(payload1, chksum1, event_ptr);
    if (event_ptr->type == BB_EVENT_ACC_RECEIVED) {
        printf("\n\t->PASSED | Result = %d", event_ptr->type);
    } else {
        printf("\n\t->FAILED | Result = %d", event_ptr->type);
    }

    printf("\n\tTest 2:");
    x = Message_ParseMessage(payload2, chksum2, event_ptr);
    if (event_ptr->type == BB_EVENT_RES_RECEIVED) {
        printf("\n\t->PASSED | Result = %d", event_ptr->type);
    } else {
        printf("\n\t->FAILED | Result = %d", event_ptr->type);
    }

    printf("\n\n\t[Testing Message_Decode]");

    y = 0;
    event_ptr->type = BB_EVENT_NO_EVENT;

    printf("\n\tTest 1:");
    while (event_ptr->type == BB_EVENT_NO_EVENT) {
        char_in = testMessage4[y];
        y++;
        Message_Decode(char_in, event_ptr);
    }
    if (event_ptr->type == BB_EVENT_SHO_RECEIVED) {
        printf("\n\t->PASSED | Result = %d", event_ptr->type);
    } else {
        printf("\n\t->FAILED | Result = %d", event_ptr->type);
    }

    event_ptr->type = BB_EVENT_NO_EVENT;
    y = 0;

    printf("\n\tTest 2:");
    while (event_ptr->type == BB_EVENT_NO_EVENT) {
        char_in = testMessage2[y];
        y++;
        Message_Decode(char_in, event_ptr);
    }
    if (event_ptr->type == BB_EVENT_RES_RECEIVED) {
        printf("\n\t->PASSED | Result = %d", event_ptr->type);
    } else {
        printf("\n\t->FAILED | Result = %d", event_ptr->type);
    }

    event_ptr->type = BB_EVENT_NO_EVENT;
    y = 0;

    printf("\n\tTest 3:");
    while (event_ptr->type == BB_EVENT_NO_EVENT) {
        char_in = testMessage3[y];
        y++;
        Message_Decode(char_in, event_ptr);
    }
    if (event_ptr->type == BB_EVENT_REV_RECEIVED) {
        printf("\n\t->PASSED | Result = %d", event_ptr->type);
    } else {
        printf("\n\t->FAILED | Result = %d", event_ptr->type);
    }

    while (1);
}
