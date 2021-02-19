#ifndef __GAME_STATE__
#define __GAME_STATE__

#pragma once

#include <snes.h>


/*---------------------------------------------------------------------------------

---------------------------------------------------------------------------------*/
#define MENU 0
#define GAMEPLAY 1

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define VRAM_SPRITE 0x6000
#define VRAM_BG_TILE 0x3000
#define VRAM_BG_MAP 0x1000

/*---------------------------------------------------------------------------------

---------------------------------------------------------------------------------*/
typedef struct
{
	int timer;
} Timing;

/*---------------------------------------------------------------------------------

---------------------------------------------------------------------------------*/
typedef struct
{
	int posX;
	int posY;
	Timing timing;
	int mySprite;
	int state;
	int second;
	int isRunning;

} GameState;

/*---------------------------------------------------------------------------------

---------------------------------------------------------------------------------*/
typedef struct {
	u8* gfxoffset;
	u16 adrgfxvram;
} Sprite;



u8 myRand(void);
void DBG_DisplayGame();
void DBG_ShowGrids();
void ClearText();
int Evaluate(int board[]);
int IsMovesLeft(int board[]);
int MinMax(int board[], int depth, int isMax, int alpha, int beta);
void AITurn_Level0();
void AITurn_Level1();
void Set();
void Reset();
void AITurn();
void TryClic(int x, int y);
void UpdateInputs();
void UpdateGameplay(GameState* gameState);
void UpdateMenu(GameState* gameState);
void Init(GameState* gameState);
void Update(GameState* gameState);
void DeInit(GameState* gameState);

#endif // __GAME_STATE__