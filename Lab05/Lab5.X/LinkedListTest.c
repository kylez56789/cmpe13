/*
 * File:   LinkedListTest.c
 * Author: Kyle Zhang | kmzhang
 *
 * Created on May 2, 2019, 2:26 PM
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xc.h"
#include "BOARD.h"
#include "LinkedList.h"



int listsize;

int main(void)
{
    BOARD_Init();
    ListItem *listptr1;
    ListItem *listptr2;
    ListItem *listptr3;
    ListItem *listptr4;
    ListItem *listptr5;
    ListItem *listptr6;
    char string1[] = "string1";
    char string2[] = "string2";
    char string3[] = "string3";
    listptr1 = LinkedListNew(string1);
    printf("Testing LinkedListNew\n");
    if (listptr1 != NULL) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    listptr2 = LinkedListNew(NULL);
    printf("Testing with data = NULL\n");
    if (listptr2 != NULL) {
        printf("PASSED\n");
    } else {
        printf("FAILED");
    }
    printf("Testing LinkedListCreateAfter\n");
    listptr3 = LinkedListCreateAfter(listptr1, string2);
    if (listptr3 != NULL) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("Testing with *item = NULL\n");
    listptr4 = LinkedListCreateAfter(NULL, string2);
    if (listptr4 != NULL) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing LinkedListRemove\n");
    listptr6 = LinkedListCreateAfter(listptr1, string3);
    if (LinkedListRemove(listptr6) == string3) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing LinkedListRemove with pointer at tail of list\n");
    listptr6 = LinkedListCreateAfter(listptr3, string3);
    if (LinkedListRemove(listptr6) == string3) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing LinkedListSize\n");
    listsize = LinkedListSize(listptr1);
    if (listsize == 2) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("Testing LinkedListSize with pointer to not first item\n");
    listsize = LinkedListSize(listptr3);
    if (listsize == 2) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }

    listptr5 = LinkedListCreateAfter(listptr3, string3);

    printf("Testing LinkedListGetFirst\n");
    if (LinkedListGetFirst(listptr5) == listptr1) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing LinkedListGetLast\n");
    if (LinkedListGetLast(listptr1) == listptr5) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing LinkedListSwapData\n");
    //printf("string1 = %s\nstring2 = %s\n", listptr1->data, listptr3->data);
    LinkedListSwapData(listptr1, listptr3);
    if (listptr1->data == string2 && listptr3->data == string1) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing LinkedListPrint\n");
    LinkedListPrint(listptr5);

    free(listptr1);
    listptr1 = NULL;
    free(listptr2);
    listptr2 = NULL;
    free(listptr3);
    listptr3 = NULL;
    free(listptr4);
    listptr4 = NULL;
    free(listptr5);
    listptr5 = NULL;
    free(listptr6);
    listptr6 = NULL;
    if (listptr1 == NULL && listptr2 == NULL && listptr3 == NULL && listptr4 == NULL
            && listptr5 == NULL && listptr6 == NULL) {
        printf("Freed all mallocs\n");
    }
    else {
        printf("Did not free all mallocs\n");
    }
    while (1);
    return 0;
}
