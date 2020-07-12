/*
 * File:   rpn_test.c
 * Author: kylez
 *
 * Created on April 27, 2019, 8:06 PM
 */


#include "xc.h"
#include <stdio.h>
#include "rpn.h"

int main()
{
    BOARD_Init();
    printf("\n###### Beginning CRUZID's rpn test harness: ####\n\n");

    char test0[] = "1 1 +";
    char test1[] = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 +";
    char test2[] = "1 1 - -";
    char test3[] = "1 d 2 +";
    char test4[] = "1 0 /";
    char test5[] = "1";
    char test6[] = "2 2 - 2";
    char test7[] = "1 2 +\b\b\b\b\b";
    int a;
    double result0, result1;
    double expected0 = 2;
    double expected1 = 5;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test0);
    ;
    if (RPN_Evaluate(test0, &result0) != 0) {
        printf("FAILED, PRODUCED AN ERROR\n");
    } else if (result0 != expected0) {
        printf("FAILED, EXPECTED = %f , RESULT = %f\n", expected1, result0);
    } else {
        printf("PASSED\n");
    }
    printf("Testing RPN_Evaluate with backspaces...\n");
    ProcessBackspaces(test7);
    if (RPN_Evaluate(test7, &result1) != 0) {
        printf("FAILED, PRODUCES AN ERROR\n");
    } else if (result1 != expected1) {
        printf("FAILED, EXPECTED = %f , RESULT = %f\n", expected1, result1);
    } else {
        printf("PASSED\n");
    }
    printf("Testing RPN_Evaluate for RPN_ERROR_OVERFLOW with \"%s\"... \n", test1);
    if (RPN_Evaluate(test1, &result0) == 1) {
        printf("PASSED, RPN_ERROR_OVERFLOW\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing RPN_Evaluate for RPN_ERROR_UNDERFLOW with \"%s\"... \n", test2);
    a = RPN_Evaluate(test2, &result0);
    if (a == 2 || a == 5) {
        printf("PASSED, RPN_ERROR_UNDERFLOW\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing RPN_Evaluate for RPN_ERROR_INVALID_TOKEN with \"%s\"... \n", test3);
    if (RPN_Evaluate(test3, &result0) == 3) {
        printf("PASSED, RPN_ERROR_INVALID_TOKEN\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing RPN_Evaluate for RPN_ERROR_DIVIDE_BY_ZERO with \"%s\"... \n", test4);
    if (RPN_Evaluate(test4, &result0) == 4) {
        printf("PASSED, RPN_ERROR_DIVIDE_BY_ZERO\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing RPN_Evaluate for RPN_ERROR_TOO_FEW_ITEMS_REMAIN with \"%s\"... \n", test5);
    if (RPN_Evaluate(test5, &result0) == 5) {
        printf("PASSED, RPN_ERROR_TOO_FEW_ITEMS_REMAIN\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing RPN_Evaluate for RPN_ERROR_TOO_MANY_ITEMS_REMAIN with \"%s\"... \n", test6);
    if (RPN_Evaluate(test6, &result0) == 6) {
        printf("PASSED, RPN_ERROR_TOO_MANY_ITEMS_REMAIN\n");
    } else {
        printf("FAILED\n");


        printf("Testing ProcessBackspaces:\n");
        char test_pb1[] = "1 2 +\b\b\b\b\b";
        int poop = ProcessBackspaces(test_pb1);
        printf("result    : %s\n", test_pb1);
        printf("should be : 1 2 3 4\n");
    }
    while (1);
}
