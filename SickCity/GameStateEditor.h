#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

class GameStateEditor : public GameState
{
public:
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	// Constructor
	GameStateEditor(Game* game);
private:
	sf::View gameView;
	sf::View guiView;
};