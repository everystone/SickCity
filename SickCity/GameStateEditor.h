#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Map.h"

enum class ActionState { NONE, PANNING };
class GameStateEditor : public GameState
{
public:
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	// Constructor
	GameStateEditor(Game* game);
private:
	ActionState actionState;
	sf::View gameView;
	sf::View guiView;
	Map map;
	
	sf::Vector2i panningAnchor;
	float zoomLevel;
};