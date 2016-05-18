#pragma once
#include <stack>
#include <SFML/Graphics.hpp>

class GameState;

class Game
{
public:
	std::stack<GameState*> states;
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