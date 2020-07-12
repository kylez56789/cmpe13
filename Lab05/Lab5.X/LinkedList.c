/*
 * File:   LinkedList.c
 * Author: Kyle Zhang | kmzhang
 *
 * Created on May 2, 2019, 2:25 PM
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xc.h"
#include "LinkedList.h"
#include "BOARD.h"

ListItem *LinkedListNew(char *data)
{
    ListItem *item = malloc(sizeof (ListItem));
    if (item == NULL) {
        return NULL;
    }
    item->data = data;
    item->nextItem = NULL;
    item->previousItem = NULL;
    return item;
}

ListItem *LinkedListCreateAfter(ListItem *item, char *data)
{
    struct ListItem *Item;
    Item = LinkedListNew(data);
    if (Item == NULL) {
        return NULL;
    }
    if (item != NULL) {
        Item->previousItem = item;
        if (item->nextItem == NULL) {
            Item->nextItem = NULL;
        } else {
            Item->nextItem = item->nextItem;
            Item->nextItem->previousItem = Item;
        }
        item->nextItem = Item;
    } else {
        Item->previousItem = NULL;
        Item->nextItem = NULL;
    }
    return Item;
}

char *LinkedListRemove(ListItem *item)
{
    if (item == NULL) {
        return NULL;
    }
    char *data = item->data;
    if (item->previousItem == NULL) {
        if (item->nextItem == NULL) {
        } else {
            item->nextItem->previousItem = NULL;
        }
    } else if (item->nextItem == NULL) {
        item->previousItem->nextItem = NULL;
    } else {
        item->previousItem->nextItem = item->nextItem;
        item->nextItem->previousItem = item->previousItem;
    }
    free(item);
    item = NULL;
    return data;
}

int LinkedListSize(ListItem *list)
{
    struct ListItem *first;
    int i;
    if (list == NULL) {
        return 0;
    }
    first = LinkedListGetFirst(list);
    for (i = 0; first != NULL; first = first->nextItem, i++) {
    }
    return i;
}

ListItem *LinkedListGetFirst(ListItem *list)
{
    struct ListItem *listcpy;
    listcpy = list;
    if (listcpy == NULL) {
        return NULL;
    }
    if (listcpy->previousItem == NULL) {
        return listcpy;
    }
    for (; listcpy->previousItem != NULL; listcpy = listcpy->previousItem) {
    }
    return listcpy;
}

ListItem *LinkedListGetLast(ListItem *list)
{
    struct ListItem *listcpy;
    listcpy = list;
    if (listcpy == NULL) {
        return NULL;
    }
    if (listcpy->nextItem == NULL) {
        return listcpy;
    }
    for (; listcpy->nextItem != NULL; listcpy = listcpy->nextItem) {
    }
    return listcpy;
}

int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem)
{
    if (firstItem == NULL || secondItem == NULL) {
        return STANDARD_ERROR;
    }
    char *string;
    string = firstItem->data;
    firstItem->data = secondItem->data;
    secondItem->data = string;
    return SUCCESS;
}

int LinkedListPrint(ListItem *list)
{
    struct ListItem *item = list;
    if (list == NULL) {
        return STANDARD_ERROR;
    } else {
        printf("[");
        item = LinkedListGetFirst(list);
        while (item->nextItem != NULL) {
            if (item->data == NULL) {
                printf("(null),");
            } else {
                printf("%s,", item->data);
            }
            item = item->nextItem;
        }
        printf("%s]\n", item->data);
        return SUCCESS;
    }
}