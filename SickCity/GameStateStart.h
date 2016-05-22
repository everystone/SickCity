#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Gui.h"

class GameStateStart : public GameState
{
private:
	sf::View view;
	std::map<std::string, Gui> guiSystem;
	void loadGame();
	void newGame(std::string name);
public:
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	// Constructor
	GameStateStart(Game* game);
};