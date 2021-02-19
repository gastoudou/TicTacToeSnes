#include "GameState.h"

/*---------------------------------------------------------------------------------
main
---------------------------------------------------------------------------------*/
int main(void)
{
	GameState gameState;
	Init(&gameState);
	
	while(1)
	{
		Update(&gameState);
	}
	DeInit(&gameState);
	
	return 0;
}