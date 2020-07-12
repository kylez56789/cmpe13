// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "rpn.h"
#include "stack.h"

// **** Set macros and preprocessor directives ****
// The lab calls for 60 characters of user input
#define MAX_INPUT_LENGTH 60

int main()
{
    BOARD_Init();

    //What follows is starter code.  You will need to modify it!

    char rpn_sentence[MAX_INPUT_LENGTH + 2];
    double result;
    int a;
    printf("Welcome to kmzhang'S RPN calculator.  Compiled on %s %s", __DATE__, __TIME__);
    while (1) {

        printf("Enter floats and + - / * in RPN format:\n");

        fgets(rpn_sentence, sizeof (rpn_sentence), stdin);
        ProcessBackspaces(rpn_sentence);
        a = RPN_Evaluate(rpn_sentence, &result);
        if (a == 1) {
            printf("RPN_ERROR_STACK_OVERFLOW\n");
        } else if (a == 2) {
            printf("RPN_ERROR_STACK_UNDERFLOW\n");
        } else if (a == 3) {
            printf("RPN_ERROR_INVALID_TOKEN\n");
        } else if (a == 4) {
            printf("RPN_ERROR_DIVIDE_BY_ZERO\n");
        } else if (a == 5) {
            printf("RPN_ERROR_TOO_FEW_ITEMS_REMAIN\n");
        } else if (a == 6) {
            printf("RPN_ERROR_TOO_MANY_ITEMS_REMAIN\n");
        } else {
            printf("result = %f\n", result);
        }

        fgets(rpn_sentence, sizeof (rpn_sentence), stdin);
    }
    while (1);
}

