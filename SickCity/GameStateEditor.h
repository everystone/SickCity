#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Map.h"

enum class ActionState { NONE, PANNING, SELECTING };
class GameStateEditor : public GameState
{
private:
	ActionState actionState;
	sf::View gameView;
	sf::View guiView;
	Map map;

	sf::Vector2i panningAnchor;
	float zoomLevel;
	sf::Vector2i selectionStart;
	sf::Vector2i selectionEnd;
	Tile* currentTile;
public:
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	// Constructor
	GameStateEditor(Game* game);

};