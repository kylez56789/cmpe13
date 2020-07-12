//>>> added by autochecker: sort.c mnlichte, Lab05, Lab05_submission_2
// Heap size 1024 required!

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "LinkedList.h"

enum Smaller {
    NULL1 = 2,
    NULL2
};


// **** Declare any function prototypes here ***
ListItem *CreateUnsortedList(void); //this function has been written for you
ListItem *SelectionSort(ListItem* list);
ListItem *InsertionSort(ListItem* list);
int isSmaller(ListItem *item1, ListItem *item2);
ListItem *LinkedListCreateBefore(ListItem *item, char *data);

/* Students should not need to modify main! */
int main(void)
{
    BOARD_Init();

    printf("\n\nWelcome to CRUZID's sort.c, compiled on %s %s.\n", __DATE__, __TIME__);

    ListItem* listToSort;

    //Do a timing test 
    printf("\nStarting stopwatch for selection sort...\n");
    listToSort = CreateUnsortedList();
    __asm("nop"); //put a breakpoint on this line
    SelectionSort(listToSort);
    __asm("nop"); //put a breakpoint on this line, and record the stopwatch time
    LinkedListPrint(listToSort);


    printf("\nStarting stopwatch for insertion sort...\n");
    listToSort = CreateUnsortedList();
    __asm("nop"); //put a breakpoint on this line
    InsertionSort(listToSort);
    __asm("nop"); //put a breakpoint on this line, and record the stopwatch time
    LinkedListPrint(listToSort);

    while (1);
    return 0;
}

/**
 * SelectionSort() performs a selection sort on a passed to it.  The list is sorted alphabetically,
 * as determined by string.h's strcmp().  
 *
 * @param list Any element in a linked list to sort.  
 * @return a pointer to the head of the sorted list.
 * 
 * SelectionSort() assumes that no list items have a NULL data member.  It also assumes that 'list'
 * is a valid, non-NULL pointer to a linked list.
 * 
 * This function does not print.
 */
ListItem *SelectionSort(ListItem* list)
{
    int k;
    struct ListItem *compare;
    struct ListItem *firstUnsorted;
    for (firstUnsorted = LinkedListGetFirst(list); firstUnsorted != NULL;
            firstUnsorted = firstUnsorted->nextItem) {
        for (compare = firstUnsorted->nextItem; compare != NULL;
                compare = compare->nextItem) {
            k = isSmaller(compare, firstUnsorted);
            if (k == TRUE) {
                LinkedListSwapData(compare, firstUnsorted);
            } else if (k == NULL1) {
                LinkedListSwapData(compare, firstUnsorted);
            } else if (k == NULL2) {
            } else {
            }
        }
    }
    return 0;
}

/**
 * InsertionSort() performs an insertion sort on a passed to it.  The list is sorted alphabetically,
 * as determined by string.h's strcmp().  
 *
 * @param list Any element in a linked list to sort.  
 * @return a pointer to the head of the sorted list.
 * 
 * SelectionSort() assumes that no list items have a NULL data member.  It also assumes that 'list'
 * is a valid, non-NULL pointer to a linked list.
 * 
 * This function does not print.
 */
ListItem * InsertionSort(ListItem * list)
{
    int result;
    struct ListItem *first;
    struct ListItem *firstSortedItem;
    struct ListItem *lastUnsortedItem;
    struct ListItem *insertAfter;
    struct ListItem *temp;
    firstSortedItem = LinkedListGetLast(list);
    while (firstSortedItem->previousItem != NULL) {
        lastUnsortedItem = firstSortedItem->previousItem;
        result = isSmaller(lastUnsortedItem, firstSortedItem);
        if (result == NULL1) {
        } else if (result == NULL2) {
            first = LinkedListGetFirst(firstSortedItem);
            LinkedListRemove(firstSortedItem);
            LinkedListCreateBefore(first, NULL);
            firstSortedItem = lastUnsortedItem;
        } else if (result == TRUE) {
            firstSortedItem = lastUnsortedItem;
        } else {
            insertAfter = firstSortedItem;
            while (insertAfter->nextItem != NULL &&
                    isSmaller(lastUnsortedItem, insertAfter->nextItem) == FALSE) {
                insertAfter = insertAfter->nextItem;
            }
            temp = lastUnsortedItem;
            LinkedListRemove(lastUnsortedItem);
            LinkedListCreateAfter(insertAfter, temp->data);
        }
    }
    return 0;
}

/* CreateUnsortedList() uses the functions in the LinkedList library to
 * generate an unsorted list.
 * 
 * Students should not modify this function!
 * If an error occurs in this function, there is almost certainly 
 * a bug in your implementation of LinkedList.h.
 *   
 * @return A pointer to a ListItem which is the head of a new unsorted list. 
 */
ListItem * CreateUnsortedList(void)
/// <editor-fold defaultstate="collapsed" desc="CreateUnsortedList definition">
{
    char* wordList[] = {"decide", "toothpaste", "lowly", "robin", "reign", "crowd", "pies", "reduce", "tendency", "surround", "finger", "rake", "alleged", "hug", "nest", "punishment", "eggnog", "side", "beef", "exuberant", "sort", "scream", "zip", "hair", "ragged", "damage", "thought", "jump", "frequent", "substance", "head", "step", "faithful", "sidewalk", "pig", "raspy", "juggle", "shut", "maddening", "rock", "telephone", "selective", NULL};
    //char* wordList[] = {"D", "A", "C", "E", "B", NULL};
    //char* wordList[] = {"tyler", "allen", "brian", "kyle", "josephine", "tracy"};

    int i = 0;
    ListItem* head = LinkedListNew(wordList[i]);
    ListItem* tail = head;
    for (i = 1; wordList[i] != NULL; i++) {
        tail = LinkedListCreateAfter(tail, wordList[i]);
    }
    return head;
}
// </editor-fold>

// used to compare two nodes

int isSmaller(ListItem *item1, ListItem * item2)
{
    if (item1->data == NULL) {
        return NULL1;
    }
    if (item2->data == NULL) {
        return NULL2;
    }
    if (strcmp(item1->data, item2->data) < 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// used if any of the nodes have data that is null

ListItem * LinkedListCreateBefore(ListItem *item, char *data)
{
    ListItem *Item = malloc(sizeof (ListItem));
    Item->data = data;
    if (item != NULL) {
        Item->nextItem = item;
        if (item->previousItem == NULL) {
            Item->previousItem = NULL;
        } else {
            Item->previousItem = item->nextItem;
            Item->previousItem->nextItem = Item;
        }
        item->previousItem = Item;
    } else {
        Item->previousItem = NULL;
        Item->nextItem = NULL;
    }
    return Item;
}