// Kyle Zhang | kmzhang

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BOARD.h"
#include <xc.h>
#include <sys/attribs.h>
#include "Agent.h"
#include "Negotiation.h"
#include "Field.h"

int main(void)
{
    BOARD_Init();
    printf("\nWelcome to kmzhang's Agent test harness, compiled on %s %s\n", __DATE__, __TIME__);
    AgentInit();
    BB_Event bbevent;
    Message message;
    AgentState state;
    printf("\nStarting AgentRun Tests...\n\n");
    bbevent.type = BB_EVENT_START_BUTTON;
    message = AgentRun(bbevent);
    if (message.type == MESSAGE_CHA && message.param0 >= 0) {
        printf("AGENT_START_STATE TEST 1 PASSED\nHash = %u\n\n", message.param0);
    } else {
        printf("AGENT_START_STATE TEST 1 FAILED\n\n");
    }
    bbevent.type = BB_EVENT_ACC_RECEIVED;
    bbevent.param0 = 12341;
    message = AgentRun(bbevent);
    if (message.type == MESSAGE_REV && message.param0 >= 0 &&
            (AgentGetState() == AGENT_STATE_WAITING_TO_SEND ||
            AgentGetState() == AGENT_STATE_DEFENDING)) {
        printf("AGENT_STATE_CHALLENGING PASSED\nA = %u\n\n", message.param0);
    } else {
        printf("AGENT_STATE_CHALLENGING FAILED\n\n");
    }
    bbevent.type = BB_EVENT_MESSAGE_SENT;
    state = AGENT_STATE_WAITING_TO_SEND;
    AgentSetState(state);
    message = AgentRun(bbevent);
    if (message.type == MESSAGE_SHO && message.param0 >= 0 && message.param1 >= 0 &&
            AgentGetState() == AGENT_STATE_ATTACKING) {
        printf("AGENT_STATE_WAITING_TO_SEND PASSED\nRow = %d\nCol = %d\n\n", message.param0, message.param1);
    } else {
        printf("AGENT_STATE_WAITING_TO_SEND FAILED\n\n");
    }
    bbevent.type = BB_EVENT_RES_RECEIVED;
    bbevent.param0 = message.param0;
    bbevent.param1 = message.param1;
    bbevent.param2 = RESULT_HIT;
    message = AgentRun(bbevent);
    if (message.type == MESSAGE_NONE && AgentGetState() == AGENT_STATE_DEFENDING) {
        printf("AGENT_STATE_ATTACKING PASSED\n\n");
    } else {
        printf("AGENT_STATE_ATTACKING FAILED\n\n");
    }
    bbevent.type = BB_EVENT_SHO_RECEIVED;
    bbevent.param0 = 0;
    bbevent.param1 = 3;
    message = AgentRun(bbevent);
    if (message.type == MESSAGE_RES && message.param0 == bbevent.param0 &&
            message.param1 == bbevent.param1 && AgentGetState() == AGENT_STATE_WAITING_TO_SEND) {
        printf("AGENT_STATE_DEFENDING PASSED\nRow = %u\nCol = %u\nResult = %u\n\n", message.param0,
                message.param1, message.param2);
    } else {
        printf("AGENT_STATE_DEFENDING FAILED\n\n");
    }
    state = AGENT_STATE_ATTACKING;
    AgentSetState(state);
    bbevent.type = BB_EVENT_RES_RECEIVED;
    bbevent.param0 = 0;
    bbevent.param1 = 3;
    bbevent.param2 = RESULT_SMALL_BOAT_SUNK;
    AgentRun(bbevent);
    state = AGENT_STATE_ATTACKING;
    AgentSetState(state);

    bbevent.param0 = 2;
    bbevent.param1 = 5;
    bbevent.param2 = RESULT_MEDIUM_BOAT_SUNK;
    AgentRun(bbevent);
    state = AGENT_STATE_ATTACKING;
    AgentSetState(state);

    bbevent.param0 = 1;
    bbevent.param1 = 6;
    bbevent.param2 = RESULT_LARGE_BOAT_SUNK;
    AgentRun(bbevent);
    state = AGENT_STATE_ATTACKING;
    AgentSetState(state);

    bbevent.param0 = 5;
    bbevent.param1 = 3;
    bbevent.param2 = RESULT_HUGE_BOAT_SUNK;
    AgentRun(bbevent);

    if (AgentGetState() == AGENT_STATE_END_SCREEN) {
        printf(" TRANSITION TO END SCREEN VICTORY PASSED\n\n");
    } else {
        printf(" TRANSITION TO END SCREEN VICTORY FAILED\n\n");
    }

    for (bbevent.param0 = 0; bbevent.param0 < FIELD_ROWS; bbevent.param0++) {
        for (bbevent.param1 = 0; bbevent.param1 < FIELD_COLS; bbevent.param1++) {
            if (AgentGetState() == AGENT_STATE_END_SCREEN) {
                break;
            }
        }
        if (AgentGetState() == AGENT_STATE_END_SCREEN) {
            break;
        }
    }
    if (AgentGetState() == AGENT_STATE_END_SCREEN) {
        printf("TRANSITION TO END SCREEN DEFEAT PASSED\n\n");
    } else {
        printf("TRANSITION TO END SCREEN DEFEAT FAILED\n\n");
    }

    bbevent.type = BB_EVENT_RESET_BUTTON;
    AgentRun(bbevent);
    if (AgentGetState() == AGENT_STATE_START) {
        printf("TRANSITION TO RESET PASSED\n\n");
    } else {
        printf("TRANSITION TO RESET FAILED\n\n");
    }
    
    
    bbevent.type = BB_EVENT_CHA_RECEIVED;
    bbevent.param0 = 100;
    message = AgentRun(bbevent);
    if (message.type == MESSAGE_ACC && message.param0 >= 0 &&
            AgentGetState() == AGENT_STATE_ACCEPTING) {
        printf("AGENT_STATE_START Test 2 PASSED\nB = %u\n\n", message.param0);
    } else {
        printf("AGENT_STATE_START TEST 2 FAILED\n\n");
    }

    bbevent.type = BB_EVENT_REV_RECEIVED;
    bbevent.param0 = 10;
    message = AgentRun(bbevent);
    if (message.type == MESSAGE_NONE && (AgentGetState() == AGENT_STATE_ATTACKING ||
            AgentGetState() == AGENT_STATE_DEFENDING)) {
        printf("AGENT_STATE_ACCEPTING PASSED\n\n");
    } else {
        printf("AGENT_STATE_ACCEPTING FAILED\n\n");
    }

    state = AGENT_STATE_ACCEPTING;
    AgentSetState(state);

    bbevent.type = BB_EVENT_REV_RECEIVED;
    bbevent.param0 = 123;
    message = AgentRun(bbevent);
    if (AgentGetState() == AGENT_STATE_END_SCREEN) {
        printf(" TRANSITION TO END SCREEN CHEATING PASSED\n\n");
    } else {
        printf(" TRANSITION TO END SCREEN CHEATING FAILED\n\n");
    }
    while (1);
}