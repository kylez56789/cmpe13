#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Player.h"

#define roomfile "RoomFiles/room%u.txt"

typedef struct{
	char currtitle[GAME_MAX_ROOM_TITLE_LENGTH + 1];
	char currdesc[GAME_MAX_ROOM_DESC_LENGTH + 1];
	uint8_t northroom;
	uint8_t southroom;
	uint8_t westroom;
	uint8_t eastroom;
} Room;

static Room room;
static FILE *filestatus;

int GameGoNorth(void)
{
	char roomtoopen[20];
		fclose(filestatus);
		sprintf(roomtoopen, roomfile, room.northroom);
                room.westroom = 0;
                room.eastroom = 0;
                room.northroom = 0;
                room.southroom = 0;
		filestatus = fopen(roomtoopen, "rb");
		if (filestatus == NULL){
			return STANDARD_ERROR;
		} else {
			return SUCCESS;
		}
		return SUCCESS;
}

int GameGoEast(void)
{
	char roomtoopen[20];
		fclose(filestatus);
		sprintf(roomtoopen, roomfile, room.eastroom);
                room.westroom = 0;
                room.eastroom = 0;
                room.northroom = 0;
                room.southroom = 0;
		filestatus = fopen(roomtoopen, "rb");
		if (filestatus == NULL){
			return STANDARD_ERROR;
		} else {
			return SUCCESS;
		}
		return SUCCESS;
}

int GameGoSouth(void)
{
	char roomtoopen[20];
		fclose(filestatus);
		sprintf(roomtoopen, roomfile, room.southroom);
                room.westroom = 0;
                room.eastroom = 0;
                room.northroom = 0;
                room.southroom = 0;
		filestatus = fopen(roomtoopen, "rb");
		if (filestatus == NULL){
			return STANDARD_ERROR;
		} else {
			return SUCCESS;
		}
		return SUCCESS;
}

int GameGoWest(void)
{
	char roomtoopen[20];
		fclose(filestatus);
		sprintf(roomtoopen, roomfile, room.westroom);
                room.westroom = 0;
                room.eastroom = 0;
                room.northroom = 0;
                room.southroom = 0;
		filestatus = fopen(roomtoopen, "rb");
		if (filestatus == NULL){
			return STANDARD_ERROR;
		} else {
			return SUCCESS;
		}
		return SUCCESS;
}

int GameInit(void)
{	
	char roomtoopen[20];
	sprintf(roomtoopen, roomfile, STARTING_ROOM);
	filestatus = fopen(roomtoopen, "rb");
	if (filestatus == NULL){
		FATAL_ERROR();
		return STANDARD_ERROR;
	}
	return SUCCESS;		
}

int GameGetCurrentRoomTitle(char *title)
{
	int x = 0;
	char c;
	char *titletemp;
	int length;
	FILE * filetemp;
	filetemp = filestatus;
	titletemp = title;
	for (x = 0; x < GAME_MAX_ROOM_TITLE_LENGTH; x++, title++){
		*title = '\0';
		room.currtitle[x] = '\0';
	}
	title = titletemp;
	filestatus = filetemp;
	fseek(filestatus, 3, SEEK_SET);
	length = fgetc(filestatus);
	if (length == EOF){
		*title = '\0';
		return 0;
	}
	for (x = 0; x < length; x++, title++){
		c = fgetc(filestatus); 
		room.currtitle[x] = c;
		*title = c;
	}
	room.currtitle[x] = '\0';
	*title = '\0';
	title = titletemp;
	return length;
}

int GameGetCurrentRoomDescription(char *desc)
{
	int desclen, x = 0, itemfound, itemnotfound, itemscon, itemsreq;
	char c;
	FILE * filetemp;
	uint8_t item;
	char * desctemp;
	
	filetemp = filestatus;
	desctemp = desc;
	
	for (x = 0; x < GAME_MAX_ROOM_DESC_LENGTH; x++, desc++){
		*desc = '\0';
		room.currdesc[x] = '\0';
	}
	desc = desctemp;
	filestatus = filetemp;
	itemsreq = fgetc(filestatus);
	if (itemsreq == EOF){
		*desc = '\0';
		return 0;
	}
	if (itemsreq > 0){
		for (x = 0; x < itemsreq; x++){
			item = fgetc(filestatus);
			itemfound = FindInInventory(item);
			if (itemfound == 0){
				itemnotfound = TRUE;
				}
		}
		desclen = fgetc(filestatus);
		fseek(filestatus, desclen, SEEK_CUR);
		itemscon = fgetc(filestatus);
		fseek(filestatus, itemscon, SEEK_CUR);
		room.northroom = fgetc(filestatus);
		room.eastroom = fgetc(filestatus);
		room.southroom = fgetc(filestatus);
		room.westroom = fgetc(filestatus);
		fgetc(filestatus);
		desclen = fgetc(filestatus);
		for (x = 0; x < desclen; x++){
			c = fgetc(filestatus);
			room.currdesc[x] = c ;
		}
		room.currdesc[x] = '\0';
		itemscon = fgetc(filestatus);
		if (itemscon > 0){
		        for (x = 0; x < itemscon; x++){
                                item = fgetc(filestatus);
                                AddToInventory(item);
                        }
                }
	} else {
		desclen = fgetc(filestatus);
		for (x = 0; x < desclen; x++){	
		c = fgetc(filestatus);
			room.currdesc[x] = c;
		}
		room.currdesc[x] = '\0';
		itemscon = fgetc(filestatus);
		if (itemscon > 0){
			for (x = 0; x < itemscon; x++){
				item = fgetc(filestatus);
				AddToInventory(item);
			}
		}
                room.northroom = fgetc(filestatus);
                room.eastroom = fgetc(filestatus);
                room.southroom = fgetc(filestatus);
                room.westroom = fgetc(filestatus);
	}
	desc = desctemp;
	for (x = 0; x < desclen; x++, desc++){
		*desc = room.currdesc[x];
	}
	desc = desctemp;
	return desclen;
}

uint8_t GameGetCurrentRoomExits(void)
{	
		uint8_t exits;
		if (room.westroom != 0){
			exits |= GAME_ROOM_EXIT_WEST_EXISTS;
		}
		if (room.southroom != 0){
			exits |= GAME_ROOM_EXIT_SOUTH_EXISTS;
		}
		if (room.eastroom != 0){
			exits |= GAME_ROOM_EXIT_EAST_EXISTS;
		}
		if (room.northroom != 0){
			exits |= GAME_ROOM_EXIT_NORTH_EXISTS;
		}
		return exits;
}	
