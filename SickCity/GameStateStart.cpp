#include "stdafx.h"
#include "GameStateStart.h"
#include "GameStateEditor.h"

void GameStateStart::draw(const float dt)
{
	this->game->window.setView(this->view);
	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);

	//draw all gui elements
	for (auto gui : this->guiSystem) {
		this->game->window.draw(gui.second);
	}
}

void GameStateStart::update(const float dt) {

}

void GameStateStart::handleInput()
{
	sf::Event event;
	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->view);
	
	//while (this->game->window.pollEvent(event)){
	this->game->window.pollEvent(event);
	switch (event.type)
		{			
			case sf::Event::Closed:
			{
				game->window.close();
				break;
			}
			case sf::Event::Resized:
			{
				this->view.setSize(event.size.width, event.size.height);
				this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
				
				// Replace menu at center
				sf::Vector2f pos = sf::Vector2f(event.size.width, event.size.height);
				pos *= 0.5f;
				pos = this->game->window.mapPixelToCoords(sf::Vector2i(pos), this->view);
				this->guiSystem.at("menu").setPosition(pos);

				this->game->background.setScale(
					float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
					float(event.size.height) / float(this->game->background.getTexture()->getSize().y));
				break;
			}
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Escape) this->game->window.close();
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::string msg = this->guiSystem.at("menu").activate(mousePos);
					if (msg == "load_game")
					{
						this->loadGame();
					}
					else if (msg == "new_game")
					{
						this->newGame("test");
					}
				}
				break;
			}
		}
}

GameStateStart::GameStateStart(Game* game)
{
	// Constructor
	this->game = game;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	this->view.setSize(pos);
	pos *= 0.5f;
	this->view.setCenter(pos);

	//Create GUI
	this->guiSystem.emplace("menu", Gui(sf::Vector2f(192, 32), 4, false, game->stylesheets.at("button"),
	{ 
		std::make_pair("New Game", "new_game"),
		std::make_pair("Load Game", "load_game")		
	}));
	
	this->guiSystem.at("menu").setPosition(pos);
	this->guiSystem.at("menu").setOrigin(96, 32 * 1 / 2);
	this->guiSystem.at("menu").show();
}

void GameStateStart::loadGame()
{
	this->game->pushState(new GameStateEditor(this->game, MenuOption::LOAD));
}
void GameStateStart::newGame(std::string name) {
	this->game->pushState(new GameStateEditor(this->game, MenuOption::NEW));
}
