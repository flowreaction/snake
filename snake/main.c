/*
Aufgabe		
Autor		Florian Bopp
Datum		
Kurzbeschreibung:	
*/
#define _CRT_SECURE_NO_WARNINGS
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
/***********************external library*******************/
#include "palette.h"
/******************functionsprototypes**********************/
#include "function_prototypes.h"

HANDLE wHnd; /* write (output) handle */
HANDLE rHnd; /* read (input handle */

/******************Main function********************/
int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Please enter the difficulty you want to play in as console argument.\n(eg. enter 'snake.exe adv')\n\n");
		printf("\tfor beginner enter\t\tbeg");
		printf("\n\tfor intermediat enter\t\tint");
		printf("\n\tfor advanced enter\t\tadv");
		printf("\n\tfor expert enter\t\texp");
		return 1;
	}
	else {
		/***********************init console*******************************/

		COLORREF palette[16] = {															//background color of console
			0x00000000, 0x00800000, 0x00008000, 0x00808000,
			0x00000080, 0x00800080, 0x00008080, 0x00c0c0c0,
			0x00808080, 0x00ff0000, 0x0000ff00, 0x00ffff00,
			0x000000ff, 0x00ff00ff, 0x0000ffff, 0x00ffffff 
		};
		
		SetConsolePalette(palette, 16, 16, L"");											//console font and size (set to square font size)

		SMALL_RECT windowSize = { 0, 0, 49, 69 };											//console size
		COORD buffersize = {50, 70};

		wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
		rHnd = GetStdHandle(STD_INPUT_HANDLE);


		SetConsoleTitle("SNAKE");															//console title
		SetConsoleWindowInfo(wHnd, true, &windowSize);
		SetConsoleScreenBufferSize(wHnd, buffersize);
		
		
		/******************init variables*******************************/
		
		UCHAR playground[VMAX][HMAX] = { 0 };
		UCHAR playgroundValue[VMAX][HMAX] = { 0 };
		hScore *root = NULL;
		int speed = 40;																		//~24 fps
		int difficulty = setDifficulty(argv[1]);											//setting difficulty with function
		UINT score = 0;
		char button = 0;																	//actual arrow value
		UCHAR ch = 0;																		//catching first return value of arrow keys	(0 or 224)
		int directionNow = right;
		int loopsSinceFood = 0;																//variable to change number of blocks added with food
		int taildirection = directionNow;
		int *taildirectionP = &taildirection;
		int x = 42, y = VMAX / 2;															//starting position of snakehead
		int tailx = 35, taily = VMAX / 2;													//starting position of tailend
		int *tailxP = &tailx;
		int *tailyP = &taily;
		int *xp = &x;
		int *yp = &y;

		/***********************init functions*****************************/
		snakeBegin(playground, playgroundValue);											//copy snake to playground
		srand((int)time(NULL));																//seed for rand()
		randFoodGenerator(playgroundValue, playground);										//first food generation
		
		/**********************start condition************************/
		if (difficulty == -1) {
			fprintf(stderr, "Restart program with correct arguement...\n\n");
			exit(1);
		}
		/***********************GAME LOOP**************************************/
		while (nextBlockIsFree(*xp, *yp, directionNow, playground)){						//condition for game
			clearScreen(0, 0);																//set cursor to top of console for overwriting
			if (foodFound(directionNow, playgroundValue, x, y)) {
				score += 10;																//score if food found
				randFoodGenerator(playgroundValue, playground);								//new piece of food
				loopsSinceFood += difficulty;												//tail deleteion gets paused for number of difficulty
			}
			else if (loopsSinceFood <= 0){
				snakeTailDeletion(playground, playgroundValue, tailxP, tailyP, taildirectionP);	//taildeletion started again 
				loopsSinceFood = 0;
			}
			else{
				loopsSinceFood--;
			}
			snakeHeadMovement(directionNow, playground, playgroundValue, yp, xp);			//snake head is moved		
			Sleep(speed);																	//loop is paused
			printBoard(playground);															//board with snake is printed
			printf("\t\tScore: %d\n", score);
			if (_kbhit() != 0) {															//get keyboard input 
				ch = _getch();																//first arrow key value is caught
				if (ch == 0 || ch == 224)
					button = _getch();														//actual arrow key value is caught
				if (button == UP || button == DOWN || button == LEFT || button == RIGHT)
					if ((button == UP && directionNow == down) || (button == DOWN && directionNow == up) || (button == LEFT && directionNow == right) || (button == RIGHT && directionNow == left))
						directionNow = directionNow;
					else
						directionNow = directionOfSnake(button);							//new direction is set
			}
		}

		/************************GAME OVER******************************/
		clearScreen(0, 0);
		printBoard(playground);
		gameOver(score);
		FILE *fp = fopen("highscorelist.txt", "rb");
		if (fp == NULL) {
			fprintf(stderr, "File opening not successful");
			return -1;
		}
		root = loadHighScore(root, fp);
		root = highScoreEntry(score, root);
		printf("\n\n\tNAME:\t\t\tSCORE:\n");
		highScorePrint(root);
		fp = freopen("highscorelist.txt", "wb", fp);
		if (fp == NULL) {
			fprintf(stderr, "File opening not successful");
			return -1;
		}
		recursiveSaveHSCORE(root, fp);
		_fcloseall();
		newline();


	}
	
	return 0;
}

