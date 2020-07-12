// Allen Cheung | alkhcheu@ucsc.edu

// **** Include libraries here ****
// Standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Oled.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "Negotiation.h"
#include "Agent.h"
#include "Field.h"
#include "BattleBoats.h"

// Defines
#define CHEATING_MESSAGE "\nOPPONENT CHEATED DURING NEGOTIATION"
#define VICTORY_MESSAGE "\n\tYOU'RE A LEGEND, YOU WON!"
#define DEFEAT_MESSAGE "\n\n\t\tYOU SUCK, YOU LOST!"
#define ERROR_MESSAGE_WRONG_MESSAGE "\nOPPONENT SENT WRONG MESSAGE"

typedef enum {
    END_CHEATING,
    END_VICTORY,
    END_DEFEAT
} EndType;


// Global variables

static Field playerField;
static Field opponentField;
static AgentState state;
static EndType endType;
static NegotiationData negotiationA;
static NegotiationData negotiationB;
static NegotiationData negotiationAHash;
static GuessData playerGuess;
static GuessData returnData;
static GuessData opponentGuess;
static Message messageToSend;
static uint8_t turnCount;

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts.  At a minimum, this requires:
 *      Initializing both fields
 *      Placing boats on your field
 *      Setting the start state of the Agent SM
 * If you are using any other persistent data in Agent SM, that should be reset as well.
 * 
 * The agent can assume that stdlib's rand() function has been seeded properly, so it is not needed
 * to call srand() inside AgentInit().
 *  */
void AgentInit(void)
{
    FieldInit(&playerField, &opponentField);
    //FieldPrint_UART(&playerField, &opponentField);
    FieldAIPlaceAllBoats(&playerField);
    //FieldPrint_UART(&playerField, &opponentField);
    turnCount = 0;
    state = AGENT_STATE_START;
}

/**
 * AgentRun evolves the Agent state machine in response to an event.
 * 
 * @param  The most recently detected event
 * @return Message, a Message struct to send to the opponent. 
 * 
 * If the returned Message struct is a valid message
 * (that is, not of type MESSAGE_NONE), then it will be
 * passed to the transmission module and sent via UART.
 * This is handled at the top level! AgentRun is ONLY responsible 
 * for generating the Message struct, not for encoding or sending it.
 */
Message AgentRun(BB_Event event)
{
    if (event.type == BB_EVENT_RESET_BUTTON) {
        AgentInit();
        state = AGENT_STATE_START;
    }
    switch (state) {
    case AGENT_STATE_START:
        if (event.type == BB_EVENT_START_BUTTON) {
            negotiationA = rand();
            negotiationAHash = NegotiationHash(negotiationA);
            messageToSend.type = MESSAGE_CHA;
            messageToSend.param0 = negotiationAHash;
            state = AGENT_STATE_CHALLENGING;
            return messageToSend;
        } else if (event.type == BB_EVENT_CHA_RECEIVED) {
            negotiationAHash = event.param0;
            negotiationB = rand();
            messageToSend.type = MESSAGE_ACC;
            messageToSend.param0 = negotiationB;
            state = AGENT_STATE_ACCEPTING;
            return messageToSend;
        }
        break;

    case AGENT_STATE_CHALLENGING:
        if (event.type != BB_EVENT_ACC_RECEIVED) {
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(VICTORY_MESSAGE);
            OledUpdate();
            state = AGENT_STATE_END_SCREEN;
        }
        if (event.type == BB_EVENT_ACC_RECEIVED) {
            messageToSend.type = MESSAGE_REV;
            messageToSend.param0 = negotiationA;
            if (NegotiateCoinFlip(negotiationA, event.param0) == HEADS) {
                state = AGENT_STATE_WAITING_TO_SEND;
            } else {
                state = AGENT_STATE_DEFENDING;
            }
            return messageToSend;
        } else {
            messageToSend.type = MESSAGE_NONE;
            return messageToSend;
        }
        break;

    case AGENT_STATE_ACCEPTING:
        if (event.type == BB_EVENT_REV_RECEIVED) {
            negotiationA = event.param0;
            if (NegotiationVerify(negotiationA, negotiationAHash) == FALSE) {
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(CHEATING_MESSAGE);
                OledUpdate();
                endType = END_CHEATING;
                state = AGENT_STATE_END_SCREEN;
            } else {
                if (NegotiateCoinFlip(negotiationA, negotiationB) == HEADS) {
                    state = AGENT_STATE_DEFENDING;
                } else {
                    turnCount = turnCount + 1;
                    playerGuess = FieldAIDecideGuess(&opponentField);
                    messageToSend.type = MESSAGE_SHO;
                    messageToSend.param0 = playerGuess.row;
                    messageToSend.param1 = playerGuess.col;
                    state = AGENT_STATE_ATTACKING;
                    return messageToSend;
                }
            }
        }
        messageToSend.type = MESSAGE_NONE;
        return messageToSend;
        break;

    case AGENT_STATE_WAITING_TO_SEND:
        if (event.type == BB_EVENT_MESSAGE_SENT) {
            turnCount = turnCount + 1;
            playerGuess = FieldAIDecideGuess(&opponentField);
            messageToSend.type = MESSAGE_SHO;
            messageToSend.param0 = playerGuess.row;
            messageToSend.param1 = playerGuess.col;
            state = AGENT_STATE_ATTACKING;
            return messageToSend;
        } else {
            messageToSend.type = MESSAGE_NONE;
            return messageToSend;
        }
        break;

    case AGENT_STATE_ATTACKING:
        if (event.type == BB_EVENT_RES_RECEIVED) {
            returnData.row = event.param0;
            returnData.col = event.param1;
            returnData.result = event.param2;
            FieldUpdateKnowledge(&opponentField, &returnData);
            //FieldPrint_UART(&playerField, &opponentField);
            if (FieldGetBoatStates(&opponentField) == 0) {
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(VICTORY_MESSAGE);
                OledUpdate();
                endType = END_VICTORY;
                state = AGENT_STATE_END_SCREEN;
            } else {
                state = AGENT_STATE_DEFENDING;
            }
        }
        messageToSend.type = MESSAGE_NONE;
        return messageToSend;
        break;

    case AGENT_STATE_DEFENDING:
        if (event.type == BB_EVENT_SHO_RECEIVED) {
            opponentGuess.row = event.param0;
            opponentGuess.col = event.param1;
            FieldRegisterEnemyAttack(&playerField, &opponentGuess);
            //FieldPrint_UART(&playerField, &opponentField);
            messageToSend.type = MESSAGE_RES;
            messageToSend.param0 = opponentGuess.row;
            messageToSend.param1 = opponentGuess.col;
            messageToSend.param2 = opponentGuess.result;
            if (FieldGetBoatStates(&playerField) == 0) {
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(DEFEAT_MESSAGE);
                OledUpdate();
                endType = END_DEFEAT;
                state = AGENT_STATE_END_SCREEN;
                return messageToSend;
            } else {
                state = AGENT_STATE_WAITING_TO_SEND;
            }
            return messageToSend;
        } else {
            messageToSend.type = MESSAGE_NONE;
            return messageToSend;
        }
        break;

    case AGENT_STATE_END_SCREEN:
        messageToSend.type = MESSAGE_NONE;
        return messageToSend;
        break;
    }
    messageToSend.type = MESSAGE_NONE;
    return messageToSend;
}

/** * 
 * @return Returns the current state that AgentGetState is in.  
 * 
 * This function is very useful for testing AgentRun.
 */
AgentState AgentGetState(void)
{
    return state;
}

/** * 
 * @param Force the agent into the state given by AgentState
 * 
 * This function is very useful for testing AgentRun.
 */
void AgentSetState(AgentState newState)
{
    state = newState;
}