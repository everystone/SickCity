#pragma once
#include <stack>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Tile.h"
#include <map>
#include <string>

class GameState;

class Game
{
private:
	void loadTextures();
	void loadTiles();

public:
	const static int tileSize = 8;
	std::stack<GameState*> states;
	TextureManager texmgr;
	sf::RenderWindow window;
	sf::Sprite background;
	std::map<std::string, Tile> tileAtlas;
	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();
	Game();
	~Game();
};