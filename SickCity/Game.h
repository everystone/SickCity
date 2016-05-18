#pragma once
#include <stack>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
class GameState;

class Game
{
private:
	void loadTextures();
public:
	std::stack<GameState*> states;
	TextureManager texmgr;
	sf::RenderWindow window;
	sf::Sprite background;

	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();
	Game();
	~Game();
};