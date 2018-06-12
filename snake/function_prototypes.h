#include "structs.h"

#ifndef fProtos_h
#define fProtos_h

void clearScreen(void);
void highScorePrint(hScore *currentElement);
hScore *highScoreEntry(int score, hScore *currentElement);
void gameOver(int score);
void randFoodGenerator(UCHAR valueBoard[VMAX][HMAX], UCHAR board[VMAX][HMAX]);
int foodFound(int direction, UCHAR valueBoard[VMAX][HMAX], int x, int y);
int nextBlockIsFree(int x, int y, int direction, UCHAR board[VMAX][HMAX]);
void snakeBegin(UCHAR board[VMAX][HMAX], UCHAR valueBoard[VMAX][HMAX]);
int directionOfSnake(char directionOfKey);
void snakeHeadMovement(int direction, UCHAR board[VMAX][HMAX], UCHAR valueBoard[VMAX][HMAX], int *y, int *x);
void snakeTailDeletion(UCHAR board[VMAX][HMAX], UCHAR valueBoard[VMAX][HMAX], int *tailx, int *taily, int *taildirection);
void printBoard(UCHAR board[VMAX][HMAX]);
int setDifficulty(char *difficulty);
void flashStandardInput(void);
void newline(void);
#endif // !fProtos_h


