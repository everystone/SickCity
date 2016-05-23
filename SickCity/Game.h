#pragma once
#include <stack>
#include <SFML/Graphics.hpp>
#include <Thor/Particles.hpp>
#include <Thor/Math/Distributions.hpp>
#include <Thor/Vectors/PolarVector2.hpp>
#include <Thor/Animations.hpp>
#include <Thor/Math/Random.hpp>
#include "TextureManager.h"
#include "Tile.h"
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
	sf::RenderWindow window;
	sf::Sprite background;

	thor::ParticleSystem particleSystem;
	//DollarEmitter* emitter;

	std::map<std::string, Tile> tileAtlas;
	std::map<std::string, GuiStyle> stylesheets;
	std::map<std::string, sf::Font> fonts;

	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	void emitParticle(unsigned int index, sf::Vector2f pos, float scale);
	GameState* peekState();

	void gameLoop();
	Game();
	~Game();


};