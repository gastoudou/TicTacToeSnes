#include "GameState.h"

/*---------------------------------------------------------------------------------
globals
---------------------------------------------------------------------------------*/
extern u8 snesfont;

extern u8 cross_sprite, cross_sprite_end;
extern u8 cross_pal;
extern u8 hand_sprite, hand_sprite_end;
extern u8 hand_pal;
extern u8 round_sprite, round_sprite_end;
extern u8 round_pal;

extern u8 grid, grid_end;
extern u8 grid_pal, grid_pal_end;
extern u8 grid_map, grid_map_end;

extern u8 bg0, bg0_end;
extern u8 bg0_pal, bg0_pal_end;
extern u8 bg0_map, bg0_map_end;

const u8 maxLevel = 2;

static u8 myRandSeed1 = 0;
static u8 myRandSeed2 = 0;

static int moveUp = 0;
static int moveDown = 0;
static int moveRight = 0;
static int moveLeft = 0;
static int gameplayStarted = 0;
static int pressedA = 0;
static int canPressA = 1;
static int turn = 0;
static int showDebug = 0;
static int gameOver = 0;
static int level = 0;
static int canGoUp = 1;
static int canGoDown = 1;
static int currentRound = 0;

static int gridChecks[9] = { 0 };
static int gridIds[9] = { 0 };
static int rounds[9] = { 0 };
int gridCoords[9][2] = { 0 };

char messtxt[128];

/*---------------------------------------------------------------------------------
myRand
---------------------------------------------------------------------------------*/
u8 myRand(void)
{
	myRandSeed1 += (myRandSeed2 >> 1);
	myRandSeed2 -= (15 ^ myRandSeed1);

	return myRandSeed1;
}

/*---------------------------------------------------------------------------------
DBG_DisplayGame
---------------------------------------------------------------------------------*/
void DBG_DisplayGame()
{
	int i = 0;
	int j = 0;

	for (; i < 3; ++i)
	{
		consoleNocashMessage(" ------------- \n");
		j = 0;
		for (; j < 3; ++j)
		{
			if (gridChecks[i * 3 + j] == 1)
			{
				consoleNocashMessage(" | X");
			}
			else if (gridChecks[i * 3 + j] == -1)
			{
				consoleNocashMessage(" | O");
			}
			else
			{
				consoleNocashMessage(" |  ");
			}
		}
		consoleNocashMessage(" | \n");
	}
	consoleNocashMessage(" ------------- \n");
}


/*---------------------------------------------------------------------------------
DBG_ShowGrids
---------------------------------------------------------------------------------*/
void DBG_ShowGrids()
{
	int i = 0;
	for (; i < 9; ++i)
	{
		sprintf(messtxt, "Init grid at %i %i", gridCoords[i][0], gridCoords[i][1]);
		consoleNocashMessage(messtxt);
	}
}

/*---------------------------------------------------------------------------------
ClearText
---------------------------------------------------------------------------------*/
void ClearText()
{
	int i = 0;
	int j = 0;
	for (; i <= 27; ++i)
	{
		j = 0;
		for (; j < 32; ++j)
		{
			consoleDrawText(j, i, " ");
		}
	}
	WaitForVBlank();
}

/*---------------------------------------------------------------------------------
Evaluate
---------------------------------------------------------------------------------*/
int Evaluate(int board[])
{
	if ((gridChecks[0] == gridChecks[1] && gridChecks[0] == gridChecks[2] && gridChecks[0] == 1)
		|| (gridChecks[3] == gridChecks[4] && gridChecks[3] == gridChecks[5] && gridChecks[3] == 1)
		|| (gridChecks[6] == gridChecks[7] && gridChecks[6] == gridChecks[8] && gridChecks[6] == 1)
		|| (gridChecks[0] == gridChecks[3] && gridChecks[0] == gridChecks[6] && gridChecks[0] == 1)
		|| (gridChecks[1] == gridChecks[4] && gridChecks[1] == gridChecks[7] && gridChecks[1] == 1)
		|| (gridChecks[2] == gridChecks[5] && gridChecks[2] == gridChecks[8] && gridChecks[2] == 1)
		|| (gridChecks[0] == gridChecks[4] && gridChecks[0] == gridChecks[8] && gridChecks[0] == 1)
		|| (gridChecks[2] == gridChecks[4] && gridChecks[2] == gridChecks[6] && gridChecks[2] == 1))
	{
		return 10;
	}

	if ((gridChecks[0] == gridChecks[1] && gridChecks[0] == gridChecks[2] && gridChecks[0] == -1)
		|| (gridChecks[3] == gridChecks[4] && gridChecks[3] == gridChecks[5] && gridChecks[3] == -1)
		|| (gridChecks[6] == gridChecks[7] && gridChecks[6] == gridChecks[8] && gridChecks[6] == -1)
		|| (gridChecks[0] == gridChecks[3] && gridChecks[0] == gridChecks[6] && gridChecks[0] == -1)
		|| (gridChecks[1] == gridChecks[4] && gridChecks[1] == gridChecks[7] && gridChecks[1] == -1)
		|| (gridChecks[2] == gridChecks[5] && gridChecks[2] == gridChecks[8] && gridChecks[2] == -1)
		|| (gridChecks[0] == gridChecks[4] && gridChecks[0] == gridChecks[8] && gridChecks[0] == -1)
		|| (gridChecks[2] == gridChecks[4] && gridChecks[2] == gridChecks[6] && gridChecks[2] == -1))
	{
		return -10;
	}

	return 0;
}

/*---------------------------------------------------------------------------------
IsMovesLeft
---------------------------------------------------------------------------------*/
int IsMovesLeft(int board[])
{
	int i = 0;
	for (; i < 9; ++i)
	{
		if (board[i] == 0)
			return 1;
	}
	return 0;
}

/*---------------------------------------------------------------------------------
MinMax
---------------------------------------------------------------------------------*/
int MinMax(int board[], int depth, int isMax, int alpha, int beta)
{
	if (level == 1)
	{
		if (depth > 2)
		{
			return 0;
		}
	}

	int score = Evaluate(board);

	if (score == 10) return score;
	if (score == -10) return score;
	if (IsMovesLeft(board) == 0) return 0;

	if (isMax == 1)
	{
		int best = -124;
		int i = 0;
		for (; i < 9; ++i)
		{
			if (board[i] == 0)
			{
				board[i] = 1;
				best = MAX(best, MinMax(board, depth + 1, 0, alpha, beta));
				board[i] = 0;
				alpha = MAX(alpha, best);
				if (beta <= alpha)
					break;
			}
		}
		return best;
	}
	else
	{
		int best = 124;
		int i = 0;
		for (; i < 9; ++i)
		{
			if (board[i] == 0)
			{
				board[i] = -1;
				best = MIN(best, MinMax(board, depth + 1, 1, alpha, beta));
				board[i] = 0;
				beta = MIN(beta, best);
				if (beta <= alpha)
					break;
			}
		}
		return best;
	}
}

/*---------------------------------------------------------------------------------
AITurn_Level0
---------------------------------------------------------------------------------*/
void AITurn_Level0()
{
	int choice = myRand() % 9;

	while (gridChecks[choice] != 0)
	{
		choice = myRand() % 9;
	}

	gridChecks[choice] = -1;
	rounds[currentRound++] = choice;
	int newID = currentRound * 4;
	gridIds[choice] = newID;

	dmaCopyVram(&cross_sprite, VRAM_SPRITE + 1024 * currentRound, (&cross_sprite_end - &cross_sprite));
	oamSet(newID, gridCoords[choice][0], gridCoords[choice][1], 3, 0, 0, 128, 0);
	oamSetEx(newID, OBJ_SMALL, OBJ_SHOW);
	WaitForVBlank();
	turn = 0;
}

/*---------------------------------------------------------------------------------
AITurn_Level1
---------------------------------------------------------------------------------*/
void AITurn_Level1()
{
	int choice = 10;
	int best = 124;
	int i = 0;

	for (; i < 9; ++i)
	{
		if (gridChecks[i] == 0)
		{
			gridChecks[i] = -1;
			int potential = MinMax(gridChecks, 0, 1, -124, +124);
			gridChecks[i] = 0;

			if (potential < best)
			{
				choice = i;
				best = potential;
			}
		}
	}
	if (choice < 10)
	{
		gridChecks[choice] = -1;
		rounds[currentRound++] = choice;
		int newID = currentRound * 4;
		gridIds[choice] = newID;

		dmaCopyVram(&cross_sprite, VRAM_SPRITE + 1024 * currentRound, (&cross_sprite_end - &cross_sprite));
		oamSet(newID, gridCoords[choice][0], gridCoords[choice][1], 3, 0, 0, 128, 0);
		oamSetEx(newID, OBJ_SMALL, OBJ_SHOW);
		WaitForVBlank();
		turn = 0;
	}
	else
	{
		AITurn_Level0();
	}
}

/*---------------------------------------------------------------------------------
Set (like reset, without touching VRam)
---------------------------------------------------------------------------------*/
void Set()
{
	moveUp = 0;
	moveDown = 0;
	moveRight = 0;
	moveLeft = 0;
	gameplayStarted = 0;
	pressedA = 0;
	canPressA = 1;
	turn = 0;
	showDebug = 0;
	gameOver = 0;
	level = 0;
	canGoUp = 1;
	canGoDown = 1;
	currentRound = 0;
	myRandSeed1 = 1;
	myRandSeed2 = 5;

	int i = 0;
	for (; i < 9; ++i)
	{
		gridChecks[i] = 0;
		rounds[i] = 0;
		gridIds[i] = -1;
	}

	gridCoords[0][0] = 70;	gridCoords[0][1] = 57;
	gridCoords[1][0] = 110;	gridCoords[1][1] = 57;
	gridCoords[2][0] = 154;	gridCoords[2][1] = 57;
	gridCoords[3][0] = 70;	gridCoords[3][1] = 101;
	gridCoords[4][0] = 110;	gridCoords[4][1] = 101;
	gridCoords[5][0] = 154;	gridCoords[5][1] = 101;
	gridCoords[6][0] = 70;	gridCoords[6][1] = 143;
	gridCoords[7][0] = 110;	gridCoords[7][1] = 143;
	gridCoords[8][0] = 154;	gridCoords[8][1] = 143;
}

/*---------------------------------------------------------------------------------
Reset
---------------------------------------------------------------------------------*/
void Reset()
{
	ClearText();

	gameOver = 0;
	currentRound = 0;
	turn = 0;

	int i = 0;
	for (; i < 9; ++i)
	{
		gridChecks[i] = 0;
		rounds[i] = 0;

		if (gridIds[i] > -1)
		{
			oamSetVisible(gridIds[i], 1);
			WaitForVBlank();
		}
		gridIds[i] = -1;
	}
}

/*---------------------------------------------------------------------------------
AITurn
---------------------------------------------------------------------------------*/
void AITurn()
{
	if (level == 0)
		AITurn_Level0();
	else
		AITurn_Level1();

	//DBG_DisplayGame();
}

/*---------------------------------------------------------------------------------
TryClic
---------------------------------------------------------------------------------*/
void TryClic(int x, int y)
{
	if (turn == 1)
		return;

	int addSprite = 0;
	int spriteX = 0;
	int spriteY = 0;
	int cell = 250;

	// a | b | c
	// ---------
	// d | e | f
	// ---------
	// g | h | i

	//a: 64-53 > 100-94
	if (x > 64 && x < 100 && y > 53 && y < 94)
	{
		if (gridChecks[0] == 0)
		{
			addSprite = 1;
			spriteX = 64;
			spriteY = 53;
			gridChecks[0] = 1;
			cell = 0;
		}
	}

	//b: 102-53 > 177-94
	if (x > 102 && x < 144 && y > 53 && y < 94)
	{
		if (gridChecks[1] == 0)
		{
			addSprite = 1;
			spriteX = 102;
			spriteY = 53;
			gridChecks[1] = 1;
			cell = 1;
		}

	}

	//c: 148-53 > 182-94
	if (x > 148 && x < 182 && y > 53 && y < 94)
	{
		if (gridChecks[2] == 0)
		{
			addSprite = 1;
			spriteX = 148;
			spriteY = 53;
			gridChecks[2] = 1;
			cell = 2;
		}
	}

	//d: 108-87 > 137-120
	if (x > 64 && x < 100 && y > 97 && y < 136)
	{
		if (gridChecks[3] == 0)
		{
			addSprite = 1;
			spriteX = 64;
			spriteY = 97;
			gridChecks[3] = 1;
			cell = 3;
		}
	}

	//e: 146-87 > 177-120
	if (x > 102 && x < 144 && y > 97 && y < 136)
	{
		if (gridChecks[4] == 0)
		{
			addSprite = 1;
			spriteX = 102;
			spriteY = 97;
			gridChecks[4] = 1;
			cell = 4;
		}
	}

	//f: 181-87 > 216-120
	if (x > 148 && x < 182 && y > 97 && y < 136)
	{
		if (gridChecks[5] == 0)
		{
			addSprite = 1;
			spriteX = 148;
			spriteY = 97;
			gridChecks[5] = 1;
			cell = 5;
		}
	}

	//g: 108-125 > 137-160
	if (x > 64 && x < 100 && y > 139 && y < 178)
	{
		if (gridChecks[6] == 0)
		{
			addSprite = 1;
			spriteX = 64;
			spriteY = 139;
			gridChecks[6] = 1;
			cell = 6;
		}
	}

	//h: 146-125 > 177-160
	if (x > 102 && x < 144 && y > 139 && y < 178)
	{
		if (gridChecks[7] == 0)
		{
			addSprite = 1;
			spriteX = 102;
			spriteY = 139;
			gridChecks[7] = 1;
			cell = 7;
		}
	}

	//i: 181-125 > 216-160
	if (x > 148 && x < 182 && y > 139 && y < 178)
	{
		if (gridChecks[8] == 0)
		{
			addSprite = 1;
			spriteX = 148;
			spriteY = 139;
			gridChecks[8] = 1;
			cell = 8;
		}
	}

	if (addSprite == 1)
	{
		rounds[currentRound++] = cell;
		int newID = currentRound * 4;
		gridIds[cell] = newID;

		dmaCopyVram(&round_sprite, VRAM_SPRITE + 1024 * currentRound, (&round_sprite_end - &round_sprite));
		oamSet(newID, gridCoords[cell][0], gridCoords[cell][1], 3, 0, 0, 64, 0);
		oamSetEx(newID, OBJ_SMALL, OBJ_SHOW);
		WaitForVBlank();
		turn = 1 - turn;

		//DBG_DisplayGame();
	}
}

/*---------------------------------------------------------------------------------
UpdateInputs
---------------------------------------------------------------------------------*/
void UpdateInputs()
{
	short pad0;
	pad0 = padsCurrent(0);

	moveUp = 0;
	moveDown = 0;
	moveRight = 0;
	moveLeft = 0;
	pressedA = 0;
	gameplayStarted = 0;

	if (pad0 & KEY_UP)
	{
		moveUp = 1;
	}
	if (pad0 & KEY_DOWN)
	{
		moveDown = 1;
	}
	if (pad0 & KEY_RIGHT)
	{
		moveRight = 1;
	}
	if (pad0 & KEY_LEFT)
	{
		moveLeft = 1;
	}
	if (pad0 & KEY_A)
	{
		pressedA = 1;
	}
	if (pad0 & KEY_START)
	{
		gameplayStarted = 1;
	}
}

/*---------------------------------------------------------------------------------
UpdateGameplay
---------------------------------------------------------------------------------*/
void UpdateGameplay(GameState* gameState)
{
	if (gameState->mySprite < 0)
	{
		bgSetDisable(0);
		bgSetDisable(2); // switch between backgrounds
		bgSetEnable(1);
		oamInitGfxSet(&hand_sprite, (&hand_sprite_end - &hand_sprite), &cross_pal, 16 * 2, 0, VRAM_SPRITE, OBJ_SIZE32);

		gameState->mySprite = 0;
		oamSet(gameState->mySprite, gameState->posX, gameState->posY, 3, 0, 0, 0x0, 0);
		oamSetEx(gameState->mySprite, OBJ_SMALL, OBJ_SHOW);
		WaitForVBlank();
	}

	if (gameOver == 0)
	{
		int score = Evaluate(gridChecks);

		if (score == 10) gameOver = 2;
		if (score == -10) gameOver = 3;
	}

	int isMoveLeft = IsMovesLeft(gridChecks);

	if (isMoveLeft == 0) gameOver = 1;

	if (gameOver > 0)
	{
		bgSetEnable(0);
		consoleDrawText(4, 2, "GAME OVER");
		if (gameOver > 1)
		{
			sprintf(messtxt, gameOver == 2 ? "Rounds win!    " : "Crosses win!   ");
			consoleDrawText(4, 4, messtxt);
		}
		else
		{
			consoleDrawText(4, 4, "Tie!           ");
		}

		consoleDrawText(4, 25, "Press START to restart");

		if (gameplayStarted)
		{
			Reset();
		}

		return;
	}

	if (turn == 1) // AI TURN, pick a empty element
	{
		AITurn();
		return;
	}

	int deltaX = moveRight - moveLeft;

	gameState->posY -= moveUp - moveDown;
	gameState->posX += deltaX;

	if (pressedA == 1 && gameOver == 0)
	{
		TryClic(gameState->posX, gameState->posY);
		pressedA = 0;
	}

	if (gameState->mySprite >= 0)
	{
		oamSet(gameState->mySprite, gameState->posX, gameState->posY, 3, 0, 0, 0, 0);
	}
}

/*---------------------------------------------------------------------------------
UpdateMenu
---------------------------------------------------------------------------------*/
void UpdateMenu(GameState* gameState)
{
	if (gameState->timing.timer < 100)
	{
		int i = 0;

		consoleDrawText(3, 10, "GLM Studio presents");
		gameState->timing.timer++;
		for (i = 0; i < gameState->timing.timer / 20; ++i)
		{
			consoleDrawText(3 + i, 11, ".");
		}
		gameplayStarted = 0;
		gameState->second = 1;
	}
	else
	{
		if (gameState->second == 1)
		{
			ClearText();
			gameState->second = 0;
		}

		consoleDrawText(3, 10, "  - T I C   T A C   T O E -");
		consoleDrawText(3, 13, "Level: ");
		{
			if (level == 0)
			{
				consoleDrawText(10, 13, "Easy");
			}
			else if (level == 1)
			{
				consoleDrawText(10, 13, "Medium");
			}
			else if (level == 2)
			{
				consoleDrawText(10, 13, "Hard");
			}

			if (moveUp && canGoUp == 1)
			{
				consoleDrawText(0, 13, "                            ");
				level++;
				canGoUp = 0;
			}
			else if (moveDown && canGoDown == 1)
			{
				consoleDrawText(0, 13, "                            ");
				level--;
				canGoDown = 0;
			}

			if (moveUp == 0)
			{
				canGoUp = 1;
			}
			if (moveDown == 0)
			{
				canGoDown = 1;
			}

			if (level < 0) level = maxLevel;
			if (level > maxLevel)level = 0;
		}

		consoleDrawText(3, 15, "Press START");

		if (gameplayStarted == 1)
		{
			gameState->state = GAMEPLAY;
			ClearText();
		}
	}
}

/*---------------------------------------------------------------------------------
Init
---------------------------------------------------------------------------------*/
void Init(GameState* gameState)
{
	Set();

	consoleInit();

	consoleInitText(0, 0, &snesfont);
	consoleSetTextCol(RGB15(31, 31, 31), RGB15(0, 0, 0));

	gameState->posX = gameState->posY = 15;
	gameState->mySprite = -1;
	gameState->timing.timer = 0;
	gameState->state = MENU;
	gameState->second = 0;
	gameState->isRunning = 1;

	// leave background 0 for text
	bgInitTileSet(2, &bg0, &bg0_pal, 0, (&bg0_end - &bg0), (&bg0_pal_end - &bg0_pal), BG_16COLORS, VRAM_BG_TILE);
	bgInitTileSet(1, &grid, &grid_pal, 0, (&grid_end - &grid), (&grid_pal_end - &grid_pal), BG_16COLORS, VRAM_BG_TILE + 0x1000);

	bgInitMapSet(2, &bg0_map, (&bg0_map_end - &bg0_map), SC_32x32, VRAM_BG_MAP);
	bgInitMapSet(1, &grid_map, (&grid_map_end - &grid_map), SC_32x32, VRAM_BG_MAP + 0x0400);

	setMode(BG_MODE1, 0); bgSetDisable(1);

	setScreenOn();
	WaitForVBlank();
}

/*---------------------------------------------------------------------------------
Update
---------------------------------------------------------------------------------*/
void Update(GameState* gameState)
{
	UpdateInputs();

	if (gameState->state == MENU)
	{
		UpdateMenu(gameState);
	}
	else if (gameState->state == GAMEPLAY)
	{
		UpdateGameplay(gameState);
	}

	WaitForVBlank();
}

/*---------------------------------------------------------------------------------
DeInit
---------------------------------------------------------------------------------*/
void DeInit(GameState* gameState)
{
	ClearText();

	gameState->posX = gameState->posY = 1;
	gameState->mySprite = -1;
	gameState->timing.timer = 0;
	gameState->state = MENU;
	gameState->second = 0;
}
