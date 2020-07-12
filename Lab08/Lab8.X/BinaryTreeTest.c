// Heap size 2048 required!

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "BinaryTree.h"

// **** Set any macros or preprocessor directives here ****


// **** Define any module-level, global, or external variables here ****
static Node *node1;
static Node *node2;
static char *data1 = "ABDECFG";
static char *data2 = "!EISH54V!3UF!!!!2ARL!!!!!WP!!J!1TNDB6!X!!KC!!Y!!MGZ7!Q!!O!8!!90";
static Node *data3;
static Node *data4;
// **** Declare any function prototypes here ****

int main()
{
    BOARD_Init();

    printf("\nWelcome to kmzhang's Binary Tree test harness, compiled on %s %s\n", __DATE__, __TIME__);

    //this is a simple test binary tree, included so you can write the relatively simple TreePrint() before implementing CreateTree().  In general, you should NOT create your binary trees this way.
    //level 1 (leaf) nodes
    //    Node nodeD = {NULL, NULL, 'D'};
    //    Node nodeE = {NULL, NULL, 'E'};
    //    Node nodeF = {NULL, NULL, 'F'};
    //    Node nodeG = {NULL, NULL, 'G'};
    //    level 2 nodes:
    //    Node nodeB = {&nodeD, &nodeE, 'B'};
    //    Node nodeC = {&nodeF, &nodeG, 'C'};
    //    level 3 node:
    //    Node nodeA = {&nodeB, &nodeC, 'A'};
    //    Node * root = &nodeA;

    //    TreePrint(root, 0);
    printf("TreeCreate Test1\n");
    node1 = TreeCreate(6, data2);
    if (node1 != NULL) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("TreeCreate Test2\n");
    node2 = TreeCreate(3, data1);
    if (node2 != NULL) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("TreeCreate Test3\n");
    node2 = TreeCreate(0, NULL);
    if (node2 == NULL) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("Testing GetLeftChild and GetRightChild\n");
    data3 = GetLeftChild(node1->leftChild->leftChild);
    if (data3->data == 'S') {
        printf("PASSED\n");
    } else {
        printf("FAILED");
    }
    printf("leftchild = %c\n", data3->data);
    data4 = GetRightChild(node1->rightChild->rightChild);
    if (data4->data == 'O') {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("rightchild = %c\n", data4->data);

    data3 = GetLeftChild(node2);
    if (data3 == NULL) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    data4 = GetRightChild(node2);
    if (data4 == NULL) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }

    printf("Testing TreePrint\n");
    TreePrint(node1, 0);
    printf("\n");
    TreePrint(node2, 0);
    printf("\n");
    while (1);
}

