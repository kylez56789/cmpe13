all: rpg

rpg: Player.o Lab10_main.o Game.o
	gcc -o rpg Player.o Lab10_main.o Game.o 
Player.o: Player.c Player.h
	gcc -c Player.c Player.h
Lab_main10.o: Lab10_main.c
	gcc -c Lab10_main.c
Game.o: Game.c Game.h
	gcc -c Game.c Game.h
Clean: rm *.o rpg.exe

