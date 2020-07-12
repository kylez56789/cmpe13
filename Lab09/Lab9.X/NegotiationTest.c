// Kyle Zhang | kmzhang

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BOARD.h"
#include <xc.h>
#include <sys/attribs.h>
#include "Negotiation.h"

NegotiationData data1 = 14562;
NegotiationData data2 = 13;
NegotiationData data3 = 4712;
NegotiationData result;

int main(void)
{
    BOARD_Init();
    printf("\nWelcome to kmzhang's Negotiation test harness, compiled on %s %s\n", __DATE__, __TIME__);
    printf("Testing NegotiationHash\n");
    result = NegotiationHash(data1);
    if (result == 14742) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    result = NegotiationHash(data2);
    if (result == 169) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    result = NegotiationHash(data3);
    if (result == 11878) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("Testing NegotiationVertify\n");
    if (NegotiationVerify(14562, 14742)) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    if (NegotiationVerify(13, 169)) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    if (NegotiationVerify(4712, 11878)) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    
    printf("Testing NegotiationCoinFlip\n");
    if (NegotiateCoinFlip(14562, 13) == TAILS){
        printf("PASSED\n");
    } else{
        printf("FAILED\n");
    }
        if (NegotiateCoinFlip(14562, 4712) == TAILS){
        printf("PASSED\n");
    } else{
        printf("FAILED\n");
    }
        if (NegotiateCoinFlip(51231, 1231) == HEADS){
        printf("PASSED\n");
    } else{
        printf("FAILED\n");
    }
    while (1);
}
