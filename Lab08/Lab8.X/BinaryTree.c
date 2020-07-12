/*
 * File:   BinaryTree.c
 * Author: kylez
 *
 * Created on May 23, 2019, 9:38 PM
 */


#include "xc.h"
#include "BOARD.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "BinaryTree.h"

Node *TreeCreate(int level, const char *data)
{
    Node *node = malloc(sizeof (Node));
    if (node == NULL) {
        return NULL;
    }
    if (data == NULL) {
        return NULL;
    }
    int x;
    x = pow(2, level - 1);
    if (level == 0) {
        return NULL;
    } else {
        node->data = *data;
        node->leftChild = TreeCreate(level - 1, data + 1);
        node->rightChild = TreeCreate(level - 1, data + x);
    }
    return node;
}

Node *GetLeftChild(Node *root)
{
    if (root == NULL) {
        return NULL;
    }
    Node *node;
    node = root->leftChild;
    return node;
}

Node *GetRightChild(Node *root)
{
    if (root == NULL) {
        return NULL;
    }
    Node *node;
    node = root->rightChild;
    return node;
}

void TreePrint(Node *node, int space)
{
    int i;
    if (node == NULL) {
        return;
    } else {
        TreePrint(node->rightChild, space + LEVEL_SPACES);
        printf("\n");
        for (i = 0; i <= space; i++) {
            printf(" ");
        }
        printf("%c", node->data);
        TreePrint(node->leftChild, space + LEVEL_SPACES);
    }
}