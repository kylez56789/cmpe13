#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//CMPE13 Support Library
#include "UNIXBOARD.h"
//
//
//
//// User libraries
#include "Player.h"
#include "Game.h"

//// **** Set any macros or preprocessor directives here ****
//
//// **** Declare any data types here ****
//
//// **** Define any global or external variables here ****
static uint8_t exitsavail;
static int titlelength;
static int desclength;
static char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
static char desc[GAME_MAX_ROOM_DESC_LENGTH + 1];
static uint8_t exited = FALSE;
//// **** Declare any function prototypes here ****

int main(){
	char userinput;
	int error;
	GameInit();
	while(1){
		titlelength = GameGetCurrentRoomTitle(title);
		printf("%s\n\n", title);
		desclength = GameGetCurrentRoomDescription(desc);
		printf("%s\n", desc);
		exitsavail = GameGetCurrentRoomExits();
		while(exited == FALSE){
                if (exitsavail != 0){
                        printf("\n\nExits Available: ");
                        if (exitsavail & GAME_ROOM_EXIT_WEST_EXISTS){
                                printf("WEST  ");
                        }
                        if (exitsavail & GAME_ROOM_EXIT_EAST_EXISTS){
                                printf("EAST  ");
                        }
                        if (exitsavail & GAME_ROOM_EXIT_SOUTH_EXISTS){
                                printf("SOUTH  ");
                        }
                        if (exitsavail & GAME_ROOM_EXIT_NORTH_EXISTS){
                                printf("NORTH  ");
                        }
               	}
			printf("\n\n\n\n\n\n\nEnter n, s, e, w, or q: ");
			while (userinput != 'n' && userinput != 's' 
				&& userinput != 'e' && userinput != 'w' && userinput != 'q'){
				userinput = getchar();
			}
			printf("\n");
			if (userinput == 'n'){
				if (exitsavail & GAME_ROOM_EXIT_NORTH_EXISTS){
					error = GameGoNorth();
					if (error == STANDARD_ERROR){
						FATAL_ERROR();
					}
					exited = TRUE;
				} else {
                                        printf("Invalid Direction: %c\n", userinput);
                                }
			} else if (userinput == 's'){
				if (exitsavail & GAME_ROOM_EXIT_SOUTH_EXISTS){
					error = GameGoSouth();
					if (error == STANDARD_ERROR){
						FATAL_ERROR();
					}
					exited = TRUE;
				} else {
                                        printf("Invalid Direction: %c\n", userinput);
                                }
			} else if (userinput == 'w'){
				if (exitsavail & GAME_ROOM_EXIT_WEST_EXISTS){
					error = GameGoWest();
					if (error == STANDARD_ERROR){
						FATAL_ERROR();
					}
					exited = TRUE;
				} else {
                                        printf("Invalid Direction: %c\n", userinput);
                                }
			} else if (userinput == 'e'){
				if (exitsavail & GAME_ROOM_EXIT_EAST_EXISTS){
					error = GameGoEast();
					if (error == STANDARD_ERROR){
						FATAL_ERROR();
					}
					exited = TRUE;
				} else {
					printf("Invalid Direction: %c\n", userinput);
				}
			} else if (userinput == 'q'){
				return 0;
			}
			userinput = '\0';
		}
		exited = FALSE;
		exitsavail = 0;
	}
}
