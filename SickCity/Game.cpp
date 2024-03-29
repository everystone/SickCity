#include "stdafx.h"
#include "Game.h"
#include "GameState.h"

void Game::loadTextures()
{
	texmgr.loadTexture("grass", "media/grass.png");
	texmgr.loadTexture("forest", "media/forest.png");
	texmgr.loadTexture("water", "media/water.png");
	texmgr.loadTexture("fire", "media/fire.png");
	texmgr.loadTexture("residential", "media/residential.png");
	texmgr.loadTexture("commercial", "media/commercial.png");
	texmgr.loadTexture("industrial", "media/industrial.png");
	texmgr.loadTexture("road", "media/road.png");
	texmgr.loadTexture("bridge", "media/bridge.png");
	texmgr.loadTexture("woodfence", "media/woodfence.png");
	texmgr.loadTexture("background", "media/background.png");
	texmgr.loadTexture("particle", "media/particle.png");
	texmgr.loadTexture("dollar", "media/dollar.png");
}

void Game::loadTiles()
{
	Animation staticAnim(0, 0, 1.0f);
	this->tileAtlas["grass"] =
		Tile(this->tileSize, 1, texmgr.getRef("grass"),
		{ staticAnim },
			TileType::GRASS, 50, 0, 1);
	tileAtlas["forest"] =
		Tile(this->tileSize, 1, texmgr.getRef("forest"),
		{ staticAnim },
			TileType::FOREST, 100, 0, 1);
	tileAtlas["water"] =
		Tile(this->tileSize, 1, texmgr.getRef("water"),
		{ Animation(0, 3, 0.5f),
			Animation(0, 3, 0.5f),
			Animation(0, 3, 0.5f) },
			TileType::WATER, 0, 0, 1);
	tileAtlas["fire"] =
		Tile(this->tileSize, 1, texmgr.getRef("fire"),
		{ Animation(0, 3, 0.2f) }, TileType::FIRE, 5, 0, 1);
	tileAtlas["residential"] =
		Tile(this->tileSize, 2, texmgr.getRef("residential"),
		{ staticAnim },
			TileType::RESIDENTIAL, 300, 25, 1);
	tileAtlas["commercial"] =
		Tile(this->tileSize, 2, texmgr.getRef("commercial"),
		{ staticAnim, staticAnim, staticAnim, staticAnim },
			TileType::COMMERCIAL, 300, 30, 4);
	tileAtlas["industrial"] =
		Tile(this->tileSize, 2, texmgr.getRef("industrial"),
		{ staticAnim, staticAnim, staticAnim,
			staticAnim },
			TileType::INDUSTRIAL, 300, 30, 4);
	tileAtlas["road"] =
		Tile(this->tileSize, 1, texmgr.getRef("road"),
		{ staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim },
			TileType::ROAD, 100, 0, 1);
	tileAtlas["bridge"] =
		Tile(this->tileSize, 1, texmgr.getRef("bridge"),
		{ staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim },
			TileType::BRIDGE, 300, 0, 1);
	tileAtlas["woodfence"] =
		Tile(this->tileSize, 1, texmgr.getRef("woodfence"),
		{ staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim },
			TileType::FENCE, 100, 0, 1);

	return;
}

void Game::loadStylesheets()
{
	this->stylesheets["button"] = GuiStyle(&this->fonts.at("main_font"), 1,
		sf::Color(0, 0, 0, 200), sf::Color(0x55, 0x55, 0x55), sf::Color(0xff, 0xff, 0xff),
		sf::Color(0x61, 0x61, 0x61), sf::Color(0x94, 0x94, 0x94), sf::Color(0x00, 0x00, 0x00));
	this->stylesheets["text"] = GuiStyle(&this->fonts.at("main_font"), 0,
		sf::Color(0x00, 0x00, 0x00, 0x00), sf::Color(255, 255, 255), sf::Color(0xff, 0xff, 0xff),
		sf::Color(0x00, 0x00, 0x00, 0x00), sf::Color(0x00, 0x00, 0x00), sf::Color(0xff, 0x00, 0x00));
	this->stylesheets["tileInfo"] = GuiStyle(&this->fonts.at("main_font"), 1,
		sf::Color(0, 0, 0, 200), sf::Color(0, 0, 0), sf::Color(0xff, 0xff, 0xff),
		sf::Color(0x61, 0x61, 0x61), sf::Color(0x94, 0x94, 0x94), sf::Color(0x00, 0x00, 0x00));
	return;
}

void Game::loadFonts()
{
	sf::Font font;
	font.loadFromFile("media/font.ttf");
	this->fonts["main_font"] = font;

	return;
}

void Game::pushState(GameState* state)
{
	this->states.push(state);
}
void Game::popState()
{
	delete this->states.top();
	this->states.pop();
}
void Game::changeState(GameState* state)
{
	if (!this->states.empty()) {
		popState();
	}
	pushState(state);
}

GameState* Game::peekState() {
	if (this->states.empty()) {
		return nullptr;
	}
	return this->states.top();
}
void Game::gameLoop()
{
	sf::Clock clock;
	while (this->window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();
		if (peekState() == nullptr) continue;
		peekState()->handleInput();
		peekState()->update(dt);
		this->window.clear(sf::Color::Black);
		peekState()->draw(dt);
		this->window.display();
	}
}
void Game::emitParticle(unsigned int index, sf::Vector2f pos, float scale)
{
	thor::UniversalEmitter emitter;

	emitter.setParticlePosition(pos);
	thor::PolarVector2f velocity(thor::random(5.0f, 20.0f), 245.0f); // thor::random(230.0f, 320.0f)
																	 //emitter.setParticleVelocity(thor::Distributions::deflect({ 0,-25 }, 10.0f));
	emitter.setParticleVelocity(velocity);
	//emitter.setEmissionRate();
	emitter.setParticleLifetime(sf::seconds(thor::random(1.0f, 1.5f)));
	emitter.setParticleScale(sf::Vector2f(scale, scale));
	//emitter.setParticleTextureInde(2);
	emitter.setParticleColor(sf::Color(100, 255, 135));
	this->particleSystem.addEmitter(emitter, sf::seconds(1));


}
Game::Game()
{
	// Constructor
	this->loadTextures();
	//this->pfx(this->texmgr);
	this->loadTiles();
	this->loadFonts();
	this->loadStylesheets();
	
	this->window.create(sf::VideoMode(800, 600), "Sick City");
	//this->window.setFramerateLimit(60);
	this->window.setVerticalSyncEnabled(true);
	this->background.setTexture(this->texmgr.getRef("background"));
	this->background.setColor(sf::Color(150, 150, 150));

	// Setup Particle Systems
	//this->pfx(this->texmgr); // attempted to move particleSystems into own class, but only got fullscreen colors, instead of particles?
	thor::FadeAnimation fader(.2f, 0.2f);
	this->particleSystem.setTexture(this->texmgr.getRef("dollar"));
	particleSystem.addAffector(thor::AnimationAffector(fader));

}

Game::~Game()
{
	// Destructor
	while (!this->states.empty()) {
		popState();
	}
}