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
		{ staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim },
			TileType::RESIDENTIAL, 300, 25, 6);
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

void Game::emitParticle(unsigned int index, sf::Vector2f pos, float scale)
{
	thor::UniversalEmitter emitter;
	
	emitter.setParticlePosition(pos);
	emitter.setParticleVelocity(thor::Distributions::deflect({ 0,-25 }, 6.0f));
	//emitter.setEmissionRate(0.5f);
	emitter.setParticleScale(sf::Vector2f(scale, scale));
	//emitter.setParticleTextureInde(2);
	emitter.setParticleColor(sf::Color(100, 255, 135));
	this->particleSystem.addEmitter(emitter, sf::seconds(1));

	//this->particleSystem.addEmitter(DollarEmitter(pos));
	
	
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
Game::Game()
{
	// Constructor
	this->loadTextures();
	this->loadTiles();
	this->loadFonts();
	this->loadStylesheets();
	this->window.create(sf::VideoMode(800, 600), "Sick City");
	//this->window.setFramerateLimit(60);
	this->window.setVerticalSyncEnabled(true);
	this->background.setTexture(this->texmgr.getRef("background"));
	this->background.setColor(sf::Color(150, 150, 150));
	
	thor::FadeAnimation fader(0.1f, 0.1f);

	this->particleSystem.setTexture(this->texmgr.getRef("dollar"));
	particleSystem.addAffector(thor::AnimationAffector(fader));
	
	//thor::UniversalEmitter emitter;
	//this->particleSystem.addEmitter(thor::refEmitter(emitter));
	
	//this->emitter.setEmissionRate(50);	
	//this->emitter.setParticleLifetime(sf::seconds(3));
	//this->particleSystem.addEmitter(this->emitter);
	//this->particleSystem.addAffector(thor::ForceAffector(sf::Vector2f(0.f, 100.f)));
	//this->particleSystem.addAffector(thor::TorqueAffector(100.f));
}

Game::~Game()
{
	// Destructor
	while (!this->states.empty()) {
		popState();
	}
}