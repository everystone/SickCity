#pragma once
#include <stack>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Tile.h"
#include <map>
#include <string>
#include "Gui.h"
#include "ParticleFX.h"


class GameState;

class Game
{
private:
	void loadTextures();
	void loadTiles();
	void loadStylesheets();
	void loadFonts();



public:
	const static int tileSize = 16;
	std::stack<GameState*> states;
	TextureManager texmgr;
	sf::RenderWindow window;
	sf::Sprite background;
	// ParticleFX pfx;
	
	thor::ParticleSystem particleSystem;
	std::map<std::string, Tile> tileAtlas;
	std::map<std::string, GuiStyle> stylesheets;
	std::map<std::string, sf::Font> fonts;

	void emitParticle(unsigned int index, sf::Vector2f pos, float scale);
	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();
	Game();
	~Game();


};