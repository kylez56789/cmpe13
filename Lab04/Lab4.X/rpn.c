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

int RPN_Evaluate(char * rpn_string, double * result)
{
    char *add = "+";
    char *subtract = "-";
    char *multiply = "*";
    char *divide = "/";
    char *zero = "0";
    char *zerodec = "0.0";
    char *zerodecneg = "-0.0";
    char *zerodecpos = "+0.0";
    struct Stack stack;
    char *token;
    double value, operand1, operand2, res;
    int limit = 2;

    StackInit(&stack);

    token = strtok(rpn_string, " ");
    value = atof(token);

    if ((value == 0) && (strcmp(token, zero) != 0) && (strcmp(token, zerodec) != 0)
            && (strcmp(token, zerodecneg) != 0) && (strcmp(token, zerodecpos) != 0)) {
        return RPN_ERROR_INVALID_TOKEN;
    }

    StackPush(&stack, value);
    while (token != NULL) {
        token = strtok(NULL, " ");
        limit++;
        if (limit > 60) { // limit of 60 characters
            return RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
        }
        if (token == NULL) {
            if (limit <= 3) {
                return RPN_ERROR_TOO_FEW_ITEMS_REMAIN;
            }
            if (StackGetSize(&stack) > 1) {
                return RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
            } else {
                StackPop(&stack, result);
                return RPN_NO_ERROR;
            }
        }
        value = atof(token); // checks validity of characters
        if ((value == 0) && (strcmp(token, zero) != 0) && (strcmp(token, add) != 0)
                && (strcmp(token, subtract) != 0) && (strcmp(token, multiply) != 0)
                && (strcmp(token, divide) != 0) && (strcmp(token, zerodec) != 0)
                && (strcmp(token, zerodecneg) != 0) && (strcmp(token, zerodecpos) != 0)) {
            return RPN_ERROR_INVALID_TOKEN;
        }
        limit++;
        if (limit > 60) {
            return RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
        }
        if ((value == 0) && (strcmp(token, zero)) != 0) {
            if (StackGetSize(&stack) < 1) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }
            StackPop(&stack, &operand1);
            if (StackGetSize(&stack) < 1) {
                return RPN_ERROR_TOO_FEW_ITEMS_REMAIN;
            }
            StackPop(&stack, &operand2);
            if (strcmp(token, add) == 0) { // checks to see which operation to perform
                res = operand1 + operand2;
            } else if (strcmp(token, subtract) == 0) {
                res = operand2 - operand1;
            } else if (strcmp(token, multiply) == 0) {
                res = operand2 * operand1;
            } else if (strcmp(token, divide) == 0) {
                res = operand2 / operand1;
                if (operand1 == 0) {
                    return RPN_ERROR_DIVIDE_BY_ZERO;
                }
            } else {
                return RPN_ERROR_INVALID_TOKEN;
            }
            StackPush(&stack, res);
        } else {
            if (StackGetSize(&stack) >= 20) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
            StackPush(&stack, value);
        }
    }
    return RPN_NO_ERROR;
}

int ProcessBackspaces(char *rpn_sentence)
{
    char backspace[2] = "\b ";
    int i, j;
    char string[2] = {}, copy[255] = {};
    string[1] = ' ';
    for (i = 0, j = 0; rpn_sentence[i] != NULL; i++, j++) {
        string[0] = rpn_sentence[i];
        if (strcmp(string, backspace) == 0) {
            j -= 2;
        } else {
            copy[j] = string[0];
        }
    }
    strcpy(rpn_sentence, copy);
    return SUCCESS;
}

