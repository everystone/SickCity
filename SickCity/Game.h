#pragma once
#include <stack>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Tile.h"
#include "Particle.h"
#include <map>
#include <string>
#include "Gui.h"

class GameState;

class Game
{
private:
	void loadTextures();
	void loadTiles();
	void loadStylesheets();
	void loadFonts();

public:
	const static int tileSize = 8;
	std::stack<GameState*> states;
	TextureManager texmgr;
	ParticleSystem particleSystem;
	sf::RenderWindow window;
	sf::Sprite background;

	std::map<std::string, Tile> tileAtlas;
	std::map<std::string, GuiStyle> stylesheets;
	std::map<std::string, sf::Font> fonts;

	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();
	Game();
	~Game();
};