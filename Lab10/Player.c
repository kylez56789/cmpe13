#include "Player.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "UNIXBOARD.h"


static uint8_t inventory[INVENTORY_SIZE];
int AddToInventory(uint8_t item)
{
	int x = 0;
	if (inventory[INVENTORY_SIZE - 1] != '\0'){
		return STANDARD_ERROR;
	}
	if (item < 0 || item > 255){
		return STANDARD_ERROR;
	}
	for (x = 0; x < INVENTORY_SIZE; x++){
		if (inventory[x] == '\0'){
			inventory[x] = item;
			break;
		}
	}
	return SUCCESS;
}



int FindInInventory(uint8_t item)
{
	int x;
	if (item < 0 || item > 255){
		return STANDARD_ERROR;
	}
	for(x = 0; x <= INVENTORY_SIZE; x++){
		if (inventory[x] == item){
			return SUCCESS;
		}
	}
	return STANDARD_ERROR;
}
