/*
Aufgabe		
Autor		Florian Bopp
Datum		
Kurzbeschreibung:	
*/
#define _CRT_SECURE_NO_WARNINGS


//#define DEBUG


/********************std headers***********************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
/*******************own libraries**********************/
#include "structs.h"
#include "enums.h"
#include "symConst.h"
#include "typedefs.h"
/******************functionsprototypes**********************/
#include "function_prototypes.h"

HANDLE wHnd; /* write (output) handle */
HANDLE rHnd; /* read (input handle */

/******************Main function********************/
int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Please enter the difficulty you want to play in as console argument.\n(eg. snake.exe adv)\n\n");
		printf("\tfor beginner enter\t\tbeg");
		printf("\n\tfor intermediat enter\t\tint");
		printf("\n\tfor advanced enter\t\tadv");
		printf("\n\tfor expert enter\t\texp");
		return 1;
	}
	else {
		/******************init variables*******************************/
		SMALL_RECT windowSize = { 0, 0, HMAX+10, VMAX+10 };
		COORD buffersize = {HMAX+11, VMAX+11};

		wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
		rHnd = GetStdHandle(STD_INPUT_HANDLE);

		SetConsoleTitle("SNAKE");
		SetConsoleWindowInfo(wHnd, true, &windowSize);
		SetConsoleScreenBufferSize(wHnd, buffersize);

		UCHAR playground[VMAX][HMAX] = { 0 };
		UCHAR playgroundValue[VMAX][HMAX] = { 0 };
		float speed = (float)setDifficulty(argv[1]);
		bool loop = true;
		int score = 0;
		char button = 0;
		int wait = 0;
		UCHAR ch = 0;
		int directionNow = right;
		int loopsSinceFood = 0;
		int directionBefore = right;
		int taildirection = directionNow;
		int *taildirectionP = &taildirection;
		int x = 42, y = VMAX / 2;
		int tailx = 35, taily = VMAX / 2;
		int *tailxP = &tailx;
		int *tailyP = &taily;
		int *xp = &x;
		int *yp = &y;

		/***********************init functions*****************************/
		snakeBegin(playground, playgroundValue);
		printBoard(playground);
		srand((int)time(NULL));
		randFoodGenerator(playgroundValue, playground);
		
		/**********************start condition************************/
		if (speed == -1) {
			fprintf(stderr, "Restart program with correct arguement...\n\n");
			exit(1);
		}
	
#ifdef DEBUG
		speed = 50;
#endif // DEBUG

		
		/***********************GAME LOOP**************************************/
		while (nextBlockIsFree(*xp, *yp, directionNow, playground)){
			if (foodFound(directionNow, playgroundValue, x, y)) {
				score += 10;
				randFoodGenerator(playgroundValue, playground);
				loopsSinceFood += ADDEDLENGTHTOSNAKE;
			}
			else if (loopsSinceFood <= 0)
			{
				snakeTailDeletion(playground, playgroundValue, tailxP, tailyP, taildirectionP);
				loopsSinceFood = 0;
			}
			else
			{
				loopsSinceFood--;
			}
			snakeHeadMovement(directionNow, playground, playgroundValue, yp, xp);					
			Sleep((int)speed / 2);
			clearScreen();
//			system("cls");
			printf("\t\t\t\t\t\t\t\tScore: %d\n", score);
			printBoard(playground);
			if (_kbhit() != 0) {
				ch = _getch();
				if (ch == 0 || ch == 224)
					button = _getch();
				if (button == UP || button == DOWN || button == LEFT || button == RIGHT)
					if ((button == UP && directionNow == down) || (button == DOWN && directionNow == up) || (button == LEFT && directionNow == right) || (button == RIGHT && directionNow == left))
						directionNow = directionNow;
					else
						directionNow = directionOfSnake(button);
			}


		}
		system("cls");
		printBoard(playground);
		gameOver(score);
		


	}
	
	return 0;
}
/********************Sichere Abfrage*******************
do {
	printf("Bitte Dimension der Matrix eingeben (1, 2 oder 3 Dimensionen): ");
	check = scanf("%lf", &dimensionen);
	flashStandardInput();
	newline();
} while (check == 0 || dimensionen < 1 || dimensionen > 3 || dimensionen != (int)dimensionen); //sichere Abfrage
*/
