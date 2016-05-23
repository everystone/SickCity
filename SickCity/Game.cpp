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
	thor::PolarVector2f velocity(thor::random(5.0f, 20.0f), thor::random(230.0f, 320.0f));
	//emitter.setParticleVelocity(thor::Distributions::deflect({ 0,-25 }, 10.0f));
	emitter.setParticleVelocity(velocity);
	//emitter.setEmissionRate();
	emitter.setParticleScale(sf::Vector2f(scale, scale));
	//emitter.setParticleTextureInde(2);
	emitter.setParticleColor(sf::Color(100, 255, 135));
	this->particleSystem.addEmitter(emitter, sf::seconds(1));	
	
	
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
	
	// Setup Particle Systems
	thor::FadeAnimation fader(.0f, 0.5f);
	this->particleSystem.setTexture(this->texmgr.getRef("dollar"));
	particleSystem.addAffector(thor::AnimationAffector(fader));
	
	
	// Weather
	this->weatherSystem.setTexture(this->texmgr.getRef("particle"));
	//this->weatherSystem.addAffector(thor::TorqueAffector(50.f));
	this->weatherSystem.addAffector(thor::ForceAffector(sf::Vector2f(10.f, 20.f)));
	this->weatherSystem.addAffector(thor::AnimationAffector(fader));

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 10; x++) {
			thor::UniversalEmitter rainEmitter;

			rainEmitter.setParticleColor(sf::Color(0, 0, 150));
			rainEmitter.setParticleScale(sf::Vector2f(.4f, .4f));
			//rainEmitter.setParticlePosition();
			rainEmitter.setEmissionRate(1);
			rainEmitter.setParticleLifetime(sf::seconds(5));
			rainEmitter.setParticlePosition(sf::Vector2f(x*this->tileSize*10, y*this->tileSize*10));
			this->weatherSystem.addEmitter(rainEmitter);
		}

	}


}

Game::~Game()
{
	// Destructor
	while (!this->states.empty()) {
		popState();
	}
}