#include "function_prototypes.h"
#include "enums.h"
#include "structs.h"
#include "symConst.h"
#include "typedefs.h"
#include "palette.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>


void clearScreen(int x, int y) {
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	
}

hScore *clearList(hScore *root) {
	if (root == NULL) return root;
	clearList(root->lower);
	clearList(root->higher);
	free(root);
	return root;
	
}

void recursiveSaveHSCORE(hScore *root, FILE *fp) {
	if (root == NULL) return;
	fwrite(root, sizeof(hScore), 1, fp);
	recursiveSaveHSCORE(root->lower, fp);
	recursiveSaveHSCORE(root->higher, fp);
}

hScore *readNextElementFromFile(hScore *currentElement, hScore *elementFromFile) {
	hScore *newElement;
	if (currentElement == NULL) {
		if ((newElement = malloc(sizeof(hScore))) == NULL) {
			exit(0);
		}
		else {
			strcpy(newElement->name, elementFromFile->name);
			newElement->score = elementFromFile->score;
			newElement->higher = NULL;
			newElement->lower = NULL;
			return newElement;
		}
	}
	else if (elementFromFile->score < currentElement->score) {
		currentElement->lower = readNextElementFromFile(currentElement->lower, elementFromFile);
		return currentElement;
	}
	else {
		currentElement->higher = readNextElementFromFile(currentElement->higher, elementFromFile);
		return currentElement;
	}
}

hScore *loadHighScore(hScore *root, FILE *fp) {
	hScore *elementFromFile = malloc(sizeof(hScore));
	root = clearList(root);
	fseek(fp, 0, SEEK_END);										//set cursor to end of file
	long fileSize = ftell(fp);									//save cursor position to filesize (cursor position = number of bytes)
	rewind(fp);													//cursor back to beginning

	int numOfItems = (int)(fileSize / (sizeof(hScore)));		//calculate number of structs that are saved in file. filesize divided by size of one struct equals number of structs
	for (int i = 0; i < numOfItems; i++) {						//for the number of structs
		fseek(fp, (sizeof(hScore) * i), SEEK_SET);			//set cursor to beginning of new struct with each iteration
		fread(elementFromFile, sizeof(hScore), 1, fp);
		root = readNextElementFromFile(root, elementFromFile);
	}
	return root;
	
}

void highScorePrint(hScore *currentElement) {
	if (currentElement == NULL) return;
	highScorePrint(currentElement->higher);
	printf("\n\t%s\t\t\t%i\n", currentElement->name, currentElement->score);
	highScorePrint(currentElement->lower);
}

hScore *highScoreEntry(UINT score, hScore *currentElement) {
	hScore *newElement;
	if (currentElement == NULL){
		if ((newElement = malloc(sizeof(hScore)))== NULL){
			exit(0);
		}
		else{
			printf("Please Enter your name (max. 5 letters): ");
			scanf("%5[^\n]s", newElement->name);
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

void randFoodGenerator(UCHAR valueBoard[VMAX][HMAX], UCHAR board[VMAX][HMAX]) { //generates random x and y coordinates and sets food to said coordinate
	int x, y;
	do{
		x = rand() % HMAX;
		y = rand() % VMAX;
	} while (board[y][x] != empty);
	valueBoard[y][x] = food;
	board[y][x] = SNAKEFOOD;
}

int foodFound(int direction, UCHAR valueBoard[VMAX][HMAX], int x, int y) {
	if (direction == up) {
		if (valueBoard[y - 1][x] == food)			//if next block contains food
			return true;
		else
			return false;
	}
	else if (direction == down) {
		if (valueBoard[y + 1][x] == food)			//if next block contains food
			return true;
		else
			return false;
	}
	else if (direction == left) {
		if (valueBoard[y][x - 1] == food)			//if next block contains food
			return true;
		else
			return false;
	}
	else if (direction == right) {
		if (valueBoard[y][x + 1] == food)			//if next block contains food
			return true;
		else
			return false;
	}
	else
		return false;
}

int nextBlockIsFree(int x, int y, int direction, UCHAR board[VMAX][HMAX]) {
	if (direction == up) {
		if (board[y - 1][x] == SNAKEBODYCHAR || (y-1) < 0)				//if next block is wall or part of snake return false
			return false;
		else
			return true;
	}
	else if (direction == down) {
		if (board[y + 1][x] == SNAKEBODYCHAR || (y+1) == VMAX)			//if next block is wall or part of snake return false
			return false;
		else
			return true;
	}
	else if (direction == left) {
		if (board[y][x - 1] == SNAKEBODYCHAR || (x-1) < 0)				//if next block is wall or part of snake return false
			return false;
		else
			return true;
	}
	else if (direction == right) {
		if (board[y][x + 1] == SNAKEBODYCHAR || (x+1) == HMAX)			//if next block is wall or part of snake return false
			return false;
		else
			return true;
	}
	else
		return true;
}

void snakeBegin(UCHAR board[VMAX][HMAX], UCHAR valueBoard[VMAX][HMAX]) { //starting position of snake
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

int directionOfSnake(char directionOfKey) {			//Key input is translatet to directional value
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
		board[*taily][*tailx] = empty;								//tail position is deleted
		valueBoard[*taily][*tailx] = empty;							//value is erased
		*taildirection = valueBoard[*taily - 1][*tailx];			//direction of tail is set to direction of next block
		*taily = *taily - 1;										//y coordinate is set to next block
	}
	else if (*taildirection == down) {
		board[*taily][*tailx] = empty;								//tail position is deleted
		valueBoard[*taily][*tailx] = empty;							//value is erased
		*taildirection = valueBoard[*taily + 1][*tailx];			//direction of tail is set to direction of next block
		*taily = *taily + 1;										//y coordinate is set to next block
	}
	else if (*taildirection == left) {
		board[*taily][*tailx] = empty;								//tail position is deleted
		valueBoard[*taily][*tailx] = empty;							//value is erased
		*taildirection = valueBoard[*taily][*tailx - 1];			//direction of tail is set to direction of next block
		*tailx = *tailx - 1;										//x coordinate is set to next block
	}
	else if (*taildirection == right) {
		board[*taily][*tailx] = empty;								//tail position is deleted
		valueBoard[*taily][*tailx] = empty;							//value is erased
		*taildirection = valueBoard[*taily][*tailx + 1];			//direction of tail is set to direction of next block
		*tailx = *tailx + 1;										//x coordinate is set to next block
	}
}

void snakeHeadMovement(int direction, UCHAR board[VMAX][HMAX], UCHAR valueBoard[VMAX][HMAX], int *y, int *x) {
	if (direction == up){
		board[*y][*x] = SNAKEBODYCHAR;							//current head position is made to body
		board[*y - 1][*x] = SNAKEHEAD;							//next empty block is set to head
		valueBoard[*y][*x] = up;								//direction is copied to valueboard
		*y = *y -1;												//y coordinate is set to next block
	}else if (direction == down){
		board[*y][*x] = SNAKEBODYCHAR;							//current head position is made to body
		board[*y + 1][*x] = SNAKEHEAD;							//next empty block is set to head
		valueBoard[*y][*x] = down;								//direction is copied to valueboard
		*y = *y + 1;											//y coordinate is set to next block
	}else if (direction == left) {
		board[*y][*x] = SNAKEBODYCHAR;							//current head position is made to body
		board[*y][*x - 1] = SNAKEHEAD;							//next empty block is set to head
		valueBoard[*y][*x] = left;								//direction is copied to valueboard
		*x = *x - 1;											//x coordinate is set to next block
	}else if (direction == right) {
		board[*y][*x] = SNAKEBODYCHAR;							//current head position is made to body
		board[*y][*x + 1] = SNAKEHEAD;							//next empty block is set to head
		valueBoard[*y][*x] = right;								//direction is copied to valueboard
		*x = *x + 1;											//x coordinate is set to next block
	}
}

int setDifficulty(char *difficulty) {								//function to return number of blocks added to snake
	if (!strcmp(difficulty, "beg"))
		return 2;
	else if (!strcmp(difficulty, "int"))
		return 5;
	else if (!strcmp(difficulty, "adv"))
		return 10;
	else if (!strcmp(difficulty, "exp"))
		return 25;
	else
		return -1;
}

void printBoard(UCHAR board[VMAX][HMAX]) {
	for (int i = 0; i <= HMAX+1; i++)											//print top line
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
	for (int i = 0; i <= HMAX+1; i++)											//print bottom line
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



/***************************external functions for console configuration*******************************/
//Source: http://cecilsunkure.blogspot.com/2011/12/windows-console-game-set-custom-color.html

/******************************************************************************
windowsVersionTest

Checks to see if have Vista/7 or earlier by attempting to retrieve function
from kernel32.dll that is only available in Vista+ version of Windows.
******************************************************************************/
int windowsVersionTest(void)
{
	/* Retrieving pointers for Windows Vista/7 Functions */
	PGetCurrentConsoleFontEx pGetCurrentConsoleFontEx = (PGetCurrentConsoleFontEx)
		GetProcAddress(GetModuleHandle("kernel32.dll"), "GetCurrentConsoleFontEx");

	/* If exists then we have Vita/7 */
	if (pGetCurrentConsoleFontEx)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/******************************************************************************
SetConsolePalette

Sets the console palette and font size.
******************************************************************************/
VOID WINAPI SetConsolePalette(COLORREF palette[], int fontX, int fontY, wchar_t *fontName)
{
	int i;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	/* Retrieving pointers for Windows Vista/7 Functions */
	PGetCurrentConsoleFontEx pGetCurrentConsoleFontEx = (PGetCurrentConsoleFontEx)
		GetProcAddress(GetModuleHandle("kernel32.dll"), "GetCurrentConsoleFontEx");
	PSetCurrentConsoleFontEx pSetCurrentConsoleFontEx = (PSetCurrentConsoleFontEx)
		GetProcAddress(GetModuleHandle("kernel32.dll"), "SetCurrentConsoleFontEx");
	PGetConsoleScreenBufferInfoEx pGetConsoleScreenBufferInfoEx = (PGetConsoleScreenBufferInfoEx)
		GetProcAddress(GetModuleHandle("kernel32.dll"), "GetConsoleScreenBufferInfoEx");
	PSetConsoleScreenBufferInfoEx pSetConsoleScreenBufferInfoEx = (PSetConsoleScreenBufferInfoEx)
		GetProcAddress(GetModuleHandle("kernel32.dll"), "SetConsoleScreenBufferInfoEx");

	/* Check for pointers: if they exist, we have Vista/7 and we can use them */
	if (pGetCurrentConsoleFontEx && pSetCurrentConsoleFontEx &&
		pGetConsoleScreenBufferInfoEx && pSetConsoleScreenBufferInfoEx)
	{
		CONSOLE_SCREEN_BUFFER_INFOEX cbi;
		CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };

		/* Tell the font info how big it is, to avoid memory corruption */
		cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		pGetCurrentConsoleFontEx(hOutput, TRUE, &cfi);

		/* Set the font type to name indicated by wide string literal */
		/* Set to 0 to keep current settings */
		lstrcpyW(cfi.FaceName, fontName);
		cfi.dwFontSize.X = fontX;
		cfi.dwFontSize.Y = fontY;
		cfi.FontFamily = 0; /* Set to 0x30 for Terminal font */
		cfi.FontWeight = 0;

		/* Set the console font info */
		pSetCurrentConsoleFontEx(hOutput, TRUE, &cfi);

		/* Get the size of the structure before filling the struct */
		cbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		pGetConsoleScreenBufferInfoEx(hOutput, &cbi);

		/* Loop through the given palette, copying the data into the color table */
		for (i = 0; i < 16; ++i)
		{
			cbi.ColorTable[i] = palette[i];
		}

		/* Set the console screen buffer info */
		pSetConsoleScreenBufferInfoEx(hOutput, &cbi);
	}
	else  /* We don't have access to the vista functions */
	{
		int i;
		CONSOLE_INFO ci = { sizeof(ci) };
		HWND hwndConsole = GetConsoleWindow();

		GetConsoleSizeInfo(&ci, hOutput);

		/* Set to 0 to keep current settings */
		ci.FontSize.X = fontX;
		ci.FontSize.Y = fontY;
		ci.FontFamily = 0; /* Set to 0x30 for Terminal font */
		ci.FontWeight = 0;

		lstrcpyW(ci.FaceName, fontName);
		ci.CursorSize = 100;
		ci.FullScreen = FALSE;
		ci.QuickEdit = FALSE;
		ci.AutoPosition = 0x10000;
		ci.InsertMode = TRUE;
		ci.ScreenColors = MAKEWORD(0x7, 0x0);
		ci.PopupColors = MAKEWORD(0x5, 0xf);
		ci.HistoryNoDup = TRUE;
		ci.HistoryBufferSize = 50;
		ci.NumberOfHistoryBuffers = 4;

		for (i = 0; i < 16; i++)
		{
			ci.ColorTable[i] = palette[i];
		}

		ci.CodePage = 0;
		ci.Hwnd = hwndConsole;
		lstrcpyW(ci.ConsoleTitle, L"");

		SetConsoleInfo(hwndConsole, &ci);
	}
}

/******************************************************************************
GetConsoleSizeInfo (XP only)

Fills up some info about the console font in the CONSOLE_INFO struct.
******************************************************************************/
static void GetConsoleSizeInfo(CONSOLE_INFO *pci, HANDLE hOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(hOutput, &csbi);
	pci->ScreenBufferSize = csbi.dwSize;
	pci->WindowSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	pci->WindowSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	pci->WindowPosX = csbi.srWindow.Left;
	pci->WindowPosY = csbi.srWindow.Top;
}

/******************************************************************************
SetConsoleInfo (XP only)

Ends up sending a message to windows to reset the console info.
******************************************************************************/
BOOL SetConsoleInfo(HWND hwndConsole, CONSOLE_INFO *pci)
{
	DWORD dwConsoleOwnerPid;
	HANDLE hProcess;
	HANDLE hSection, hDupSection;
	PVOID ptrView = 0;
	HANDLE hThread;

	/* Open the process which "owns" the console */
	GetWindowThreadProcessId(hwndConsole, &dwConsoleOwnerPid);
	hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, dwConsoleOwnerPid);


	/* Create a SECTION object backed by page-file, then map a view of  */
	/* this section into the owner process so we can write the contents  */
	/* of the CONSOLE_INFO buffer into it  */
	hSection = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, pci->Length, 0);


	/* Copy our console structure into the section-object */
	ptrView = MapViewOfFile(hSection, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, pci->Length);
	memcpy(ptrView, pci, pci->Length);
	UnmapViewOfFile(ptrView);


	/* Map the memory into owner process  */
	DuplicateHandle(GetCurrentProcess(), hSection, hProcess, &hDupSection, 0, FALSE, DUPLICATE_SAME_ACCESS);

	/* Send console window the "update" message WM_SETCONSOLEINFO */
	SendMessage(hwndConsole, WM_SETCONSOLEINFO, (WPARAM)hDupSection, 0);

	/*clean up */
	hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)CloseHandle, hDupSection, 0, 0);
	CloseHandle(hThread);
	CloseHandle(hSection);
	CloseHandle(hProcess);
	return TRUE;
}
