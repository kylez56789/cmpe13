/*
 * File:   stack_test.c
 * Author: kylez
 *
 * Created on April 27, 2019, 8:06 PM
 */


#include "xc.h"
#include <stdio.h>
#include "BOARD.h"
#include "stack.h"

int main()
{
    BOARD_Init();
    struct Stack myStack;
    struct Stack *stackptr = &myStack;
    double value, value1 = 10;
    double value2 = 20;
    double *valueptr;
    valueptr = &value;
    printf("\n###### Beginning CRUZID's stack test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!

    printf("Testing StackInit()\n");
    StackInit(stackptr);
    if (myStack.currentItemIndex == -1) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("Testing StackPush()\n");
    if (StackPush(stackptr, value1)) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("%f", stackptr->stackItems[stackptr->currentItemIndex]);
    if (StackPush(stackptr, value2)) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("%f", stackptr->stackItems[stackptr->currentItemIndex]);
    printf("Testing StackPush() with stack full\n");
    myStack.currentItemIndex = STACK_SIZE - 1;
    if (StackPush(stackptr, value1) == 0) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    StackInit(stackptr);
    StackPush(stackptr, value1);
    printf("Testing StackPop()\n");
    StackPop(stackptr, valueptr);
    if (*valueptr == 10) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing StackPop() with stack empty\n");
    StackInit(stackptr);
    if (StackPop(stackptr, valueptr)) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing StackIsEmpty()\n");
    printf("Test 1: Empty\n");
    StackInit(stackptr);
    if (StackIsEmpty(stackptr)) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    StackPush(stackptr, value1);
    printf("Test 2: Not Empty\n");
    if (StackIsEmpty(stackptr) == 0) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("Testing StackIsFull()\n");
    printf("Test 1: Not Full\n");
    if (StackIsFull(stackptr) == 0) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("Test 2: Full\n");
    myStack.currentItemIndex = STACK_SIZE - 1;
    if (StackIsFull(stackptr)) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing StackGetSize()\n");
    StackInit(stackptr);
    if (StackGetSize(stackptr) == 0) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    StackPush(stackptr, value1);
    if (StackGetSize(stackptr) == 1) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    myStack.currentItemIndex = STACK_SIZE - 22;
    if (StackGetSize(stackptr) == -1) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    while (1);
    return 0;
}
