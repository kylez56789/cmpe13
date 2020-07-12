/*
 * File:   part3.c
 * Author: kylez
 *
 * Created on April 7, 2019, 3:22 PM
 */


#include "xc.h"

int main(void)
{
    BOARD_Init();
    OledInit();
    OledDrawString("Hello, World!\n");
    OledUpdate();
    while(1);
}
