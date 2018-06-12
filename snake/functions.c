#include "function_prototypes.h"
#include "enums.h"
#include "structs.h"
#include "symConst.h"
#include "typedefs.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>

void clearScreen(void) {
	system("cls");
}


void highScorePrint(hScore *currentElement) {
	if (currentElement == NULL) return;
	highScorePrint(currentElement->higher);
	printf("\t%s\t\t\t%i\n\n", currentElement->name, currentElement->score);
	highScorePrint(currentElement->lower);
}

hScore *highScoreEntry(int score, hScore *currentElement) {
	hScore *newElement;
	
	if (currentElement == NULL){
		if ((newElement = malloc(sizeof(hScore)))== NULL){
			exit(0);
		}
		else{
			printf("Please Enter your name: ");
			scanf("%19[^\n]s", newElement->name);
			newElement->score = score;
			newElement->higher = NULL;
			newElement->lower = NULL;
			return newElement;
		}
	}
	else if (score < currentElement->score){
		currentElement->lower = highScoreEntry(score, currentElement->lower);
		return currentElement;
	}
	else{
		currentElement->higher = highScoreEntry(score, currentElement->higher);
		return currentElement;
	}
}


void gameOver(int score) {
	printf("\tGame Over!\nYour Score:\t%i\n\n", score);
}

void randFoodGenerator(UCHAR valueBoard[VMAX][HMAX], UCHAR board[VMAX][HMAX]) {
	int x, y;
	do
	{
		x = rand() % HMAX;
		y = rand() % VMAX;
		
		/*if (x <= 1)
			x += 3;
		else if (x >= HMAX-1)
			x -= 3;
		if (y <= 1)
			y += 3;
		else if (y >= VMAX-1)
			y -= 3;*/
	} while (board[y][x] != 0);

	
	valueBoard[y][x] = food;
	board[y][x] = SNAKEFOOD;
}

int foodFound(int direction, UCHAR valueBoard[VMAX][HMAX], int x, int y) {
	if (direction == up) {
		if (valueBoard[y - 1][x] == food)
			return true;
		else
			return false;
	
	}
	else if (direction == down) {
		if (valueBoard[y + 1][x] == food)
			return true;
		else
			return false;
	}
	else if (direction == left) {
		if (valueBoard[y][x - 1] == food)
			return true;
		else
			return false;
	}
	else if (direction == right) {
		if (valueBoard[y][x + 1] == food)
			return true;
		else
			return false;
	}
	else
		return false;
}

int nextBlockIsFree(int x, int y, int direction, UCHAR board[VMAX][HMAX]) {
	if (direction == up) {
		if (board[y - 1][x] == SNAKEBODYCHAR || (y-1) < 0)
			return false;
		else
			return true;
	}
	else if (direction == down) {
		if (board[y + 1][x] == SNAKEBODYCHAR || (y+1) == VMAX)
			return false;
		else
			return true;
	}
	else if (direction == left) {
		if (board[y][x - 1] == SNAKEBODYCHAR || (x-1) < 0)
			return false;
		else
			return true;
	}
	else if (direction == right) {
		if (board[y][x + 1] == SNAKEBODYCHAR || (x+1) == HMAX)
			return false;
		else
			return true;
	}
	else
		return true;
}

void snakeBegin(UCHAR board[VMAX][HMAX], UCHAR valueBoard[VMAX][HMAX]) {
	board[VMAX / 2][35] = SNAKEBODYCHAR;
	board[VMAX / 2][36] = SNAKEBODYCHAR;
	board[VMAX / 2][37] = SNAKEBODYCHAR;
	board[VMAX / 2][38] = SNAKEBODYCHAR;
	board[VMAX / 2][39] = SNAKEBODYCHAR;
	board[VMAX / 2][40] = SNAKEBODYCHAR;
	board[VMAX / 2][41] = SNAKEBODYCHAR;
	board[VMAX / 2][42] = SNAKEHEAD;

	valueBoard[VMAX / 2][35] = right;
	valueBoard[VMAX / 2][36] = right;
	valueBoard[VMAX / 2][37] = right;
	valueBoard[VMAX / 2][38] = right;
	valueBoard[VMAX / 2][39] = right;
	valueBoard[VMAX / 2][40] = right;
	valueBoard[VMAX / 2][41] = right;
	valueBoard[VMAX / 2][42] = right;

}

int directionOfSnake(char directionOfKey) {
	if (directionOfKey == UP)
		return up;
	else if (directionOfKey == DOWN)
		return down;
	else if (directionOfKey == LEFT)
		return left;
	else
		return right;
}

void snakeTailDeletion(UCHAR board[VMAX][HMAX], UCHAR valueBoard[VMAX][HMAX], int *tailx, int *taily, int *taildirection) {
	if (*taildirection == up){
		board[*taily][*tailx] = 0;
		valueBoard[*taily][*tailx] = 0;
		*taildirection = valueBoard[*taily - 1][*tailx];
		*taily = *taily - 1;
	}
	else if (*taildirection == down) {
		board[*taily][*tailx] = 0;
		valueBoard[*taily][*tailx] = 0;
		*taildirection = valueBoard[*taily + 1][*tailx];
		*taily = *taily + 1;
	}
	else if (*taildirection == left) {
		board[*taily][*tailx] = 0;
		valueBoard[*taily][*tailx] = 0;
		*taildirection = valueBoard[*taily][*tailx - 1];
		*tailx = *tailx - 1;
	}
	else if (*taildirection == right) {
		board[*taily][*tailx] = 0;
		valueBoard[*taily][*tailx] = 0;
		*taildirection = valueBoard[*taily][*tailx + 1];
		*tailx = *tailx + 1;
	}
}

void snakeHeadMovement(int direction, UCHAR board[VMAX][HMAX], UCHAR valueBoard[VMAX][HMAX], int *y, int *x) {
	if (direction == up){
		board[*y][*x] = SNAKEBODYCHAR;
		board[*y - 1][*x] = SNAKEHEAD;
		valueBoard[*y][*x] = up;
		*y = *y -1;
	}else if (direction == down){
		board[*y][*x] = SNAKEBODYCHAR;
		board[*y + 1][*x] = SNAKEHEAD;
		valueBoard[*y][*x] = down;
		*y = *y + 1;
	}else if (direction == left) {
		board[*y][*x] = SNAKEBODYCHAR;
		board[*y][*x - 1] = SNAKEHEAD;
		valueBoard[*y][*x] = left;
		*x = *x - 1;
	}else if (direction == right) {
		board[*y][*x] = SNAKEBODYCHAR;
		board[*y][*x + 1] = SNAKEHEAD;
		valueBoard[*y][*x] = right;
		*x = *x + 1;
	}
}

int setDifficulty(char *difficulty) {
	if (!strcmp(difficulty, "beg"))
		return 750;
	else if (!strcmp(difficulty, "int"))
		return 500;
	else if (!strcmp(difficulty, "adv"))
		return 350;
	else if (!strcmp(difficulty, "exp"))
		return 200;
	else
		return -1;
}

void printBoard(UCHAR board[VMAX][HMAX]) {
	for (int i = 0; i <= HMAX+1; i++)
		printf("%c", 95);
	newline();
	for (int i = 0; i < VMAX; i++)
	{
		printf("|");
		for (int j = 0; j < HMAX; j++)
		{
			printf("%c", board[i][j]);
		}
		printf("|\n");

	}
	for (int i = 0; i <= HMAX+1; i++)
		printf("%c", 238);
	newline();
}

void flashStandardInput(void)
{
	int intCharacter;
	while ((intCharacter = getchar()) != '\n' && intCharacter != EOF);
}

void newline(void) {
	printf("\n");
}
