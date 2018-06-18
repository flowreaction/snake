#include "structs.h"
#include "palette.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#ifndef fProtos_h
#define fProtos_h

hScore *clearList(hScore *root);
void recursiveSaveHSCORE(hScore *root, FILE *fp);
hScore *readNextElementFromFile(hScore *currentElement, hScore *elementFromFile);
hScore *loadHighScore(hScore *root, FILE *fp);
void clearScreen(int x, int y);
void highScorePrint(hScore *currentElement);
hScore *highScoreEntry(UINT score, hScore *currentElement);
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

int windowsVersionTest(void);
VOID WINAPI SetConsolePalette(COLORREF palette[], int fontX, int fontY, wchar_t *fontName);
static void GetConsoleSizeInfo(CONSOLE_INFO *pci, HANDLE hOutput);
BOOL SetConsoleInfo(HWND hwndConsole, CONSOLE_INFO *pci);
#endif // !fProtos_h


