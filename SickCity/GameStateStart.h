#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

class GameStateStart : public GameState
{
public:
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	// Constructor
	GameStateStart(Game* game);
private:
	sf::View view;
	void loadGame();
};