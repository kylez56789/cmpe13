// Allen Cheung | alkhcheu@ucsc.edu

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// Battleboats Libraries:
#include "Message.h"
#include "Field.h"

int main(void)
{
    BOARD_Init();
    printf("\nWelcome to alkhcheu's Field test harness, compiled on %s %s\n", __DATE__, __TIME__);
    
    // FIELD INIT
    printf("\n\t[Testing FieldInit]");
    printf("\n\tTest 1: ");
    Field playerTestField;
    Field opponentTestField;
    FieldInit(&playerTestField, &opponentTestField);
    int passed = TRUE;
    int i = 0;
    int j = 0;
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            if (playerTestField.grid[i][j] != FIELD_SQUARE_EMPTY) {
                passed = FALSE;
            }
        }
    }
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            if (opponentTestField.grid[i][j] != FIELD_SQUARE_UNKNOWN) {
                passed = FALSE;
            }
        }
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    printf("\n\tTest 2: ");
    passed = TRUE;
    if (opponentTestField.smallBoatLives != FIELD_BOAT_SIZE_SMALL) {
        passed = FALSE;
    } else if (opponentTestField.mediumBoatLives != FIELD_BOAT_SIZE_MEDIUM) {
        passed = FALSE;
    } else if (opponentTestField.largeBoatLives != FIELD_BOAT_SIZE_LARGE) {
        passed = FALSE;
    } else if (opponentTestField.hugeBoatLives != FIELD_BOAT_SIZE_HUGE) {
        passed = FALSE;
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    
    // SET SQUARE STATUS
    printf("\n\n\t[Testing FieldSetSquareStatus]");
    printf("\n\tTest 1: ");
    passed = TRUE;
    if (FieldSetSquareStatus(&playerTestField, 3, 4, FIELD_SQUARE_SMALL_BOAT) != FIELD_SQUARE_EMPTY) {
        if (playerTestField.grid[3][4] != FIELD_SQUARE_SMALL_BOAT) {
            passed = FALSE;
        }
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    FieldSetSquareStatus(&playerTestField, 3, 4, FIELD_SQUARE_EMPTY);
    printf("\n\tTest 2:");
    passed = TRUE;
    if (FieldSetSquareStatus(&playerTestField, 7, 12, 2) != FIELD_SQUARE_INVALID) {
        passed = FALSE;
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    printf("\n\tTest 3:");
    passed = TRUE;
    if (FieldSetSquareStatus(&opponentTestField, 5, 9, FIELD_SQUARE_MISS) != FIELD_SQUARE_UNKNOWN) {
        if (opponentTestField.grid[6][10] != FIELD_SQUARE_MISS) {
            passed = FALSE;
        }
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }

    // GET SQUARE STATUS
    printf("\n\n\t[Testing FieldGetSquareStatus]");
    printf("\n\tTest 1: ");
    passed = TRUE;
    if (FieldGetSquareStatus(&playerTestField, 3, 4) != FIELD_SQUARE_EMPTY) {
        passed = FALSE;
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    passed = TRUE;
    printf("\n\tTest 2: ");
    if (FieldGetSquareStatus(&opponentTestField, 5, 9) != FIELD_SQUARE_MISS) {
        passed = FALSE;
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    FieldSetSquareStatus(&opponentTestField, 5, 9, FIELD_SQUARE_UNKNOWN);
    printf("\n\tTest 3: ");
    passed = TRUE;
    if (FieldGetSquareStatus(&playerTestField, 15, 13) != FIELD_SQUARE_INVALID) {
        passed = FALSE;
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    
    // ADD BOAT
    printf("\n\n\t[Testing FieldAddBoat]");
    printf("\n\tTest 1: ");
    passed = TRUE;
    FieldAddBoat(&playerTestField, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    FieldAddBoat(&playerTestField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);
    FieldAddBoat(&playerTestField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_HUGE);
    FieldAddBoat(&playerTestField, 0, 6, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_SMALL);
    for (i = 0; i < 3; i++) {
        if (FieldGetSquareStatus(&playerTestField, 0, i) != FIELD_SQUARE_SMALL_BOAT) {
            passed = FALSE;
        }
    }
    for (i = 0; i < 4; i++) {
        if (FieldGetSquareStatus(&playerTestField, 1, i) != FIELD_SQUARE_MEDIUM_BOAT) {
            passed = FALSE;
        }
    }
    for (i = 0; i < 3; i++) {
        if (FieldGetSquareStatus(&playerTestField, i, 6) != FIELD_SQUARE_SMALL_BOAT) {
            passed = FALSE;
        }
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    printf("\n\tTest 2: ");
    passed = TRUE;
    if (FieldAddBoat(&playerTestField, 5, 7, FIELD_DIR_EAST, FIELD_BOAT_TYPE_HUGE) != STANDARD_ERROR) {
        passed = FALSE;
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    printf("\n\tTest 3: ");
    passed = TRUE;
    if (FieldAddBoat(&playerTestField, 5, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM) != SUCCESS) {
        passed = FALSE;
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }

    // REGISTER ENEMY ATTACK
    printf("\n\n\t[Testing FieldRegisterEnemyAttack]");
    printf("\n\tTest 1: ");
    passed = TRUE;
    GuessData opponentTestGuess;
    opponentTestGuess.row = 0;
    opponentTestGuess.col = 0;
    FieldRegisterEnemyAttack(&playerTestField, &opponentTestGuess);
    if (FieldGetSquareStatus(&playerTestField, 0, 0) != FIELD_SQUARE_HIT) {
        if (opponentTestGuess.result != RESULT_HIT) {
            passed = FALSE;
        }
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    printf("\n\tTest 2: ");
    passed = TRUE;
    opponentTestGuess.row = 0;
    opponentTestGuess.col = 6;
    FieldRegisterEnemyAttack(&playerTestField, &opponentTestGuess);
    opponentTestGuess.row = 1;
    opponentTestGuess.col = 6;
    FieldRegisterEnemyAttack(&playerTestField, &opponentTestGuess);
    opponentTestGuess.row = 2;
    opponentTestGuess.col = 6;
    FieldRegisterEnemyAttack(&playerTestField, &opponentTestGuess);
    for (i = 0; i < 3; i++) {
        if (FieldGetSquareStatus(&playerTestField, i, 6) != FIELD_SQUARE_HIT) {
            passed = FALSE;
        }
    }
    opponentTestGuess.row = 0;
    opponentTestGuess.col = 0;
    FieldRegisterEnemyAttack(&playerTestField, &opponentTestGuess);
    opponentTestGuess.row = 0;
    opponentTestGuess.col = 1;
    FieldRegisterEnemyAttack(&playerTestField, &opponentTestGuess);
    opponentTestGuess.row = 0;
    opponentTestGuess.col = 2;
    FieldRegisterEnemyAttack(&playerTestField, &opponentTestGuess);
    if (opponentTestGuess.result != RESULT_SMALL_BOAT_SUNK) {
        passed = FALSE;
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    
    // GET BOAT STATES
    printf("\n\n\t[Testing FieldGetBoatStates]");
    printf("\n\tTest 1: ");
    passed = TRUE;
    Field playerTestField2;
    Field opponentTestField2;
    FieldInit(&playerTestField2, &opponentTestField2);
    GuessData enemyGuess;
    FieldAddBoat(&playerTestField2, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    enemyGuess.row = 0;
    enemyGuess.col = 0;
    FieldRegisterEnemyAttack(&playerTestField2, &enemyGuess);
    enemyGuess.row = 0;
    enemyGuess.col = 1;
    FieldRegisterEnemyAttack(&playerTestField2, &enemyGuess);
    enemyGuess.row = 0;
    enemyGuess.col = 2;
    if (FieldGetBoatStates(&playerTestField2) != 0x01) {
        passed = FALSE;
    }
    FieldRegisterEnemyAttack(&playerTestField2, &enemyGuess);
    if (FieldGetBoatStates(&playerTestField2) != 0) {
        passed = FALSE;
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    printf("\n\tTest 2: ");
    passed = TRUE;
    FieldAddBoat(&playerTestField2, 0, 4, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_HUGE);
    enemyGuess.row = 0;
    enemyGuess.col = 4;
    FieldRegisterEnemyAttack(&playerTestField2, &enemyGuess);
    enemyGuess.row = 1;
    enemyGuess.col = 4;
    FieldRegisterEnemyAttack(&playerTestField2, &enemyGuess);
    enemyGuess.row = 2;
    enemyGuess.col = 4;
    FieldRegisterEnemyAttack(&playerTestField2, &enemyGuess);
    enemyGuess.row = 3;
    enemyGuess.col = 4;
    FieldRegisterEnemyAttack(&playerTestField2, &enemyGuess);
    enemyGuess.row = 4;
    enemyGuess.col = 4;
    FieldRegisterEnemyAttack(&playerTestField2, &enemyGuess);
    enemyGuess.row = 5;
    enemyGuess.col = 4;
    if (FieldGetBoatStates(&playerTestField2) != 0x08) {
        passed = FALSE;
    }
    FieldRegisterEnemyAttack(&playerTestField2, &enemyGuess);
    if (FieldGetBoatStates(&playerTestField2) != 0) {
        passed = FALSE;
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }

    
    // AI DECIDE GUESS
    printf("\n\n\t[Testing FieldAIDecideGuess]");
    printf("\n\tTest 1:");
    Field testGuessField;
    Field opponentGuessField;
    FieldInit(&testGuessField, &opponentGuessField);
    
    GuessData guessAI;
    GuessData returnAI;
    
    guessAI = FieldAIDecideGuess(&opponentGuessField);
    printf("\nRow = %u", guessAI.row);
    printf("\nCol = %u\n", guessAI.col);
    returnAI.row = guessAI.row;
    returnAI.col = guessAI.col;
    returnAI.result = RESULT_HIT;
    FieldUpdateKnowledge(&opponentGuessField, &returnAI);
    FieldPrint_UART(&testGuessField, &opponentGuessField);

    guessAI = FieldAIDecideGuess(&opponentGuessField);
    printf("\nRow = %u", guessAI.row);
    printf("\nCol = %u\n", guessAI.col);
    returnAI.row = guessAI.row;
    returnAI.col = guessAI.col;
    returnAI.result = RESULT_HIT;
    FieldUpdateKnowledge(&opponentGuessField, &returnAI);
    FieldPrint_UART(&testGuessField, &opponentGuessField);

    guessAI = FieldAIDecideGuess(&opponentGuessField);
    printf("\nRow = %u", guessAI.row);
    printf("\nCol = %u\n", guessAI.col);
    returnAI.row = guessAI.row;
    returnAI.col = guessAI.col;
    returnAI.result = RESULT_HIT;
    FieldUpdateKnowledge(&opponentGuessField, &returnAI);
    FieldPrint_UART(&testGuessField, &opponentGuessField);

    guessAI = FieldAIDecideGuess(&opponentGuessField);
    printf("\nRow = %u", guessAI.row);
    printf("\nCol = %u\n", guessAI.col);
    returnAI.row = guessAI.row;
    returnAI.col = guessAI.col;
    returnAI.result = RESULT_MEDIUM_BOAT_SUNK;
    FieldUpdateKnowledge(&opponentGuessField, &returnAI);
    FieldPrint_UART(&testGuessField, &opponentGuessField);
    
    
    guessAI = FieldAIDecideGuess(&opponentGuessField);
    printf("\nRow = %u", guessAI.row);
    printf("\nCol = %u\n", guessAI.col);
    returnAI.row = guessAI.row;
    returnAI.col = guessAI.col;
    returnAI.result = RESULT_HIT;
    FieldUpdateKnowledge(&opponentGuessField, &returnAI);
    FieldPrint_UART(&testGuessField, &opponentGuessField);
    
    
    guessAI = FieldAIDecideGuess(&opponentGuessField);
    printf("\nRow = %u", guessAI.row);
    printf("\nCol = %u\n", guessAI.col);
    returnAI.row = guessAI.row;
    returnAI.col = guessAI.col;
    returnAI.result = RESULT_MISS;
    FieldUpdateKnowledge(&opponentGuessField, &returnAI);
    FieldPrint_UART(&testGuessField, &opponentGuessField);
    
    guessAI = FieldAIDecideGuess(&opponentGuessField);
    printf("\nRow = %u", guessAI.row);
    printf("\nCol = %u\n", guessAI.col);
    returnAI.row = guessAI.row;
    returnAI.col = guessAI.col;
    returnAI.result = RESULT_HIT;
    FieldUpdateKnowledge(&opponentGuessField, &returnAI);
    FieldPrint_UART(&testGuessField, &opponentGuessField);
    
    
    // UPDATE KNOWLEDGE
    printf("\n\n\t[Testing FieldUpdateKnowledge]");
    printf("\n\tTest 1: ");
    passed = TRUE;
    GuessData opponentReturnData;
    opponentReturnData.row = 5;
    opponentReturnData.col = 6;
    opponentReturnData.result = RESULT_HIT;
    if (FieldUpdateKnowledge(&opponentTestField, &opponentReturnData) != FIELD_SQUARE_EMPTY) {
        if (FieldGetSquareStatus(&opponentTestField, 5, 6) != FIELD_SQUARE_HIT) {
            passed = FALSE;
        }
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    FieldSetSquareStatus(&opponentTestField, 5, 6, FIELD_SQUARE_UNKNOWN);
    printf("\n\tTest 2: ");
    passed = TRUE;
    opponentReturnData.row = 2;
    opponentReturnData.col = 5;
    opponentReturnData.result = RESULT_MISS;
    if (FieldUpdateKnowledge(&opponentTestField, &opponentReturnData) != FIELD_SQUARE_EMPTY) {
        if (FieldGetSquareStatus(&opponentTestField, 2, 5) != FIELD_SQUARE_MISS) {
            passed = FALSE;
        }
    }
    if (passed == TRUE) {
        printf("\n\t->PASSED");
    } else {
        printf("\n\t->FAILED");
    }
    
    // AI PLACE ALL BOATS
    printf("\n\n\t[Testing FieldAIPlaceAllBoats]");
    printf("\n\tTest 1: \n");
    Field testAIField;
    Field testOppField;
    FieldInit(&testAIField, &testOppField);
    FieldAIPlaceAllBoats(&testAIField);
    FieldPrint_UART(&testAIField, &testOppField);
    printf("Finished");

    while (1);
}
