/*
 * File:   part0.c
 * Author: kylez
 *
 * Created on April 3, 2019, 6:28 PM
 */

#include "stdio.h"
#include "xc.h"
#include "BOARD.h"

void main(void)
{
    BOARD_Init();
    printf("Hello World\n");
    while(1);
}
