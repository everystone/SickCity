// SickCity.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GameStateStart.h"
#include "Game.h"

int main()
{
	Game game;
	game.pushState(new GameStateStart(&game));
	game.gameLoop();	
    return 0;
}

