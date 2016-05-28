#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "City.h"
#include "Player.h"

enum class ActionState { NONE, PANNING, SELECTING, MENU };
class GameStateEditor : public GameState
{
private:
	ActionState actionState;
	sf::View gameView;
	sf::View guiView;
	City city;
	Player localPlayer;
	sf::Vector2i panningAnchor;
	float zoomLevel;
	sf::Vector2i selectionStart;
	sf::Vector2i selectionEnd;
	Tile* currentTile;
	std::map<std::string, Gui> guiSystem;
public:
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	// Constructor
	GameStateEditor(Game* game, MenuOption choice, std::string name = "");

};