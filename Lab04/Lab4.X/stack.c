// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "stack.h"

void StackInit(struct Stack * stack)
{
    stack->currentItemIndex = -1;
    stack->initialized = TRUE;
}

int StackPush(struct Stack *stack, double value)
{
    if (stack->initialized != TRUE) {
        return STANDARD_ERROR;
    }
    stack->currentItemIndex++;
    stack->stackItems[stack->currentItemIndex] = value;
    if (StackIsFull(stack)) { // uses stackisfull function to check if stack is full
        return STANDARD_ERROR;
    } else {
        return SUCCESS;
    }
}

int StackPop(struct Stack *stack, double *value)
{
    double *valueptr;
    valueptr = &*value;
    if (stack->initialized != TRUE) {
        return STANDARD_ERROR;
    }
    if (StackIsEmpty(stack)) { // uses stackisempty function to check if stack is empty
        return STANDARD_ERROR;
    } else {
        return SUCCESS;
    }
    *valueptr = stack->stackItems[stack->currentItemIndex];
    stack->currentItemIndex--;
}

int StackIsEmpty(const struct Stack * stack)
{
    if (stack->currentItemIndex == -1 && stack->initialized == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int StackIsFull(const struct Stack * stack)
{
    if (stack->currentItemIndex >= STACK_SIZE - 1 && stack->initialized == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int StackGetSize(const struct Stack * stack)
{
    if (stack->initialized != TRUE) {
        return SIZE_ERROR;
    } else {
        return stack->currentItemIndex + 1;
    }
}

