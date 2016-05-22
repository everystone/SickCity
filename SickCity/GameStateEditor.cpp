#include "stdafx.h"
#include "GameState.h"
#include "GameStateEditor.h"
#include <map>

#include "Gui.h"

void GameStateEditor::draw(const float dt) {
   this->game->window.clear(sf::Color::Black);

    this->game->window.setView(this->guiView);
    this->game->window.draw(this->game->background);

    this->game->window.setView(this->gameView);
    this->city.map.draw(this->game->window, dt);
	this->game->window.draw(this->game->particleSystem);

	this->game->window.setView(this->guiView);
	for (auto gui : this->guiSystem) {
		this->game->window.draw(gui.second);
	}
    return;
}

void GameStateEditor::update(const float dt) {
	this->city.update(dt);

	/* Update info bar at the bottom of the screen */
	this->guiSystem.at("infoBar").setEntryText(0, "Day: " + std::to_string(this->city.day));
	this->guiSystem.at("infoBar").setEntryText(1, "$" + std::to_string(this->city.funds));
	this->guiSystem.at("infoBar").setEntryText(2, "Pop: " +std::to_string(long(this->city.population)) + " ( " + std::to_string(long(this->city.getHomeless())) + ")");
	this->guiSystem.at("infoBar").setEntryText(3, "Emp:: " + std::to_string(long(this->city.employable)) + " ( " + std::to_string(long(this->city.getUnemployed())) + ")");
	this->guiSystem.at("infoBar").setEntryText(4, "Tile: " +tileTypeToStr(currentTile->tileType));
	
	/* Highlight entries of the right click context menu */
	this->guiSystem.at("rightClickMenu").
		highlight(this->guiSystem.at("rightClickMenu")
		.getEntry(this->game->window.mapPixelToCoords(
			sf::Mouse::getPosition(this->game->window), this->guiView))
		);

	/* Update Particle SYstem */
	this->game->particleSystem.update(dt);
	return;
}

void GameStateEditor::handleInput()
{
	sf::Event event;
	sf::Vector2f guiPos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView);
	sf::Vector2f gamePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
	
	//while (this->game->window.pollEvent(event)) {
	this->game->window.pollEvent(event);
		switch (event.type)
		{
		case sf::Event::Closed:
			this->game->window.close();
			break;
		case sf::Event::Resized:
		{
			gameView.setSize(event.size.width, event.size.height);
			gameView.zoom(zoomLevel);
			guiView.setSize(event.size.width, event.size.height);
			this->guiSystem.at("infoBar").setDimensions(sf::Vector2f(event.size.width / this->guiSystem.at("infoBar").entries.size(), 16));
			this->guiSystem.at("infoBar").setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, event.size.height - 16), this->guiView));
			this->guiSystem.at("infoBar").show();
						
			this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0), this->guiView));
			this->game->background.setScale(
				float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
				float(event.size.height) / float(this->game->background.getTexture()->getSize().y));
			break;
		}
		case sf::Event::MouseMoved:
		{
			/* Pan the camera */
			if(this->actionState == ActionState::PANNING)
			{
				sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(this->game->window) - this->panningAnchor);
				gameView.move(-1.0f * pos * this->zoomLevel);
				panningAnchor = sf::Mouse::getPosition(this->game->window);
			}
			/* Select Tiles*/
			else if (this->actionState == ActionState::SELECTING)
			{
				sf::Vector2f pos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
				selectionEnd.x = pos.y / (this->city.map.tileSize) + pos.x / (2 * this->city.map.tileSize) - this->city.map.width * 0.5 - 0.5;
				selectionEnd.y = pos.y / (this->city.map.tileSize) - pos.x / (2 * this->city.map.tileSize) + this->city.map.width * 0.5 + 0.5;
				
				this->city.map.clearSelected();
				if (this->currentTile->tileType == TileType::GRASS)
				{
					this->city.map.select(selectionStart, selectionEnd, { this->currentTile->tileType, TileType::WATER, TileType::FIRE });
				}
				else {
					this->city.map.select(selectionStart, selectionEnd,
					{
						this->currentTile->tileType,    TileType::FOREST,
						TileType::WATER,                TileType::ROAD,
						TileType::RESIDENTIAL,          TileType::COMMERCIAL,
						TileType::INDUSTRIAL
					});
				}

				// Update GUI
				unsigned int selectionCost = this->currentTile->cost * this->city.map.numSelected;
				this->guiSystem.at("selectionCostText").setEntryText(0, "$" + std::to_string(selectionCost));
				if (this->city.funds < selectionCost) {
					this->guiSystem.at("selectionCostText").highlight(0);
				}
				else {
					this->guiSystem.at("selectionCostText").highlight(-1);
				}
				this->guiSystem.at("selectionCostText").setPosition(guiPos + sf::Vector2f(16, -16));
				this->guiSystem.at("selectionCostText").show();
			}
			else if(this->actionState == ActionState::NONE) {
				// Show overlay of TileInfo if mouse hovers over tile, other than grass.
				selectionEnd.x = gamePos.y / (this->city.map.tileSize) + gamePos.x / (2 * this->city.map.tileSize) - this->city.map.width * 0.5 - 0.5;
				selectionEnd.y = gamePos.y / (this->city.map.tileSize) - gamePos.x / (2 * this->city.map.tileSize) + this->city.map.width * 0.5 + 0.5;
				//std::cout << "selection: " << selectionEnd.x << ", " << selectionEnd.y << std::endl;
				if (selectionEnd.x >= 0 && selectionEnd.x < this->city.map.width && selectionEnd.y >= 0 && selectionEnd.y < this->city.map.height) {
					// check if index is lower than vector size
					int index = selectionEnd.y*this->city.map.width + selectionEnd.x;
					//if (index < this->city.map.tiles.size())
						// Set map.hovered to index, for tile to be drawn in other color
						this->city.map.hovered = index;

						Tile& hoveredTile = this->city.map.tiles[index];
						this->guiSystem.at("tileInfo").setPosition(guiPos + sf::Vector2f(32, -16));
						this->guiSystem.at("tileInfo").setEntryText(0, tileTypeToStr(hoveredTile.tileType));
						this->guiSystem.at("tileInfo").setEntryText(1, "Lvl: " + std::to_string(hoveredTile.tileVariant));
						this->guiSystem.at("tileInfo").setEntryText(2, "Pop: " + std::to_string((int)hoveredTile.population));
						this->guiSystem.at("tileInfo").setEntryText(3, "Prod: " + std::to_string((int)hoveredTile.production));
						this->guiSystem.at("tileInfo").setEntryText(4, "Stored: " + std::to_string((int)hoveredTile.storedGoods));
						//this->guiSystem.at("tileInfo").setEntryText(5, "Res: " + std::to_string(this->city.map.resources[index]));
						this->guiSystem.at("tileInfo").setEntryText(5, "Region: " + std::to_string(hoveredTile.regions[0]));
						this->guiSystem.at("tileInfo").show();
						//std::cout << "Tile: " << tileTypeToStr(hovered.tileType) << "(" << index << ")" << std::endl;
				}
				else {
					this->guiSystem.at("tileInfo").hide();
				}
			}
			// highlight entries of right click context menu
			this->guiSystem.at("rightClickMenu").highlight(this->guiSystem.at("rightClickMenu").getEntry(guiPos));

			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			/* Start panning */
			if(event.mouseButton.button == sf::Mouse::Right)
			{
				this->guiSystem.at("rightClickMenu").hide();
				this->guiSystem.at("selectionCostText").hide();
				this->guiSystem.at("tileInfo").hide();
				if(this->actionState != ActionState::PANNING)
				{
					this->actionState = ActionState::PANNING;
					this->panningAnchor = sf::Mouse::getPosition(this->game->window);
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				this->guiSystem.at("tileInfo").hide();
				/* Select a context menu entry */
				if (this->guiSystem.at("rightClickMenu").visible == true)
				{
					std::string msg = this->guiSystem.at("rightClickMenu").activate(guiPos);
					if (msg != "null") this->currentTile = &this->game->tileAtlas.at(msg);

					this->guiSystem.at("rightClickMenu").hide();
					this->actionState = ActionState::NONE;
					return;
				}
				/* select map tile */
				if (this->actionState != ActionState::SELECTING)
				{

					this->actionState = ActionState::SELECTING;					
					selectionStart.x = gamePos.y / (this->city.map.tileSize) + gamePos.x / (2 * this->city.map.tileSize) - this->city.map.width * 0.5 - 0.5;
					selectionStart.y = gamePos.y / (this->city.map.tileSize) - gamePos.x / (2 * this->city.map.tileSize) + this->city.map.width * 0.5 + 0.5;
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Middle)
			{
				/* Stop selecting */
				this->guiSystem.at("tileInfo").hide();
				if (this->actionState == ActionState::SELECTING)
				{
					//this->actionState = ActionState::MENU;;
					this->guiSystem.at("selectionCostText").hide();

					this->city.map.clearSelected();
				}
				else {
					// Open the tile select menu
					sf::Vector2f pos = guiPos;
					if (pos.x > this->game->window.getSize().x - this->guiSystem.at("rightClickMenu").getSize().x)
					{
						pos -= sf::Vector2f(this->guiSystem.at("rightClickMenu").getSize().x, 0);
					}
					if (pos.y > this->game->window.getSize().y - this->guiSystem.at("rightClickMenu").getSize().y)
					{
						pos -= sf::Vector2f(0, this->guiSystem.at("rightClickMenu").getSize().y);
					}
					this->guiSystem.at("rightClickMenu").setPosition(pos);
					this->guiSystem.at("rightClickMenu").show();
				}
				this->actionState = ActionState::MENU;
			}
			else if (event.mouseButton.button == sf::Mouse::XButton1) {
				std::cout << "particles\n";
				this->game->particleSystem.setPosition(gamePos);
				this->game->particleSystem.fuel(50);
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			/* Stop panning */
			if(event.mouseButton.button == sf::Mouse::Right)
			{
				this->actionState = ActionState::NONE;
			}
			// Stop selecting
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (this->actionState == ActionState::SELECTING)
				{
					/* Replace tiles if enough funds and a tile is selected */
					if (this->currentTile != nullptr) {
						unsigned int cost = this->currentTile->cost * this->city.map.numSelected;
						if (this->city.funds >= cost) {
							this->city.bulldoze(*this->currentTile);
							this->city.funds -= this->currentTile->cost * this->city.map.numSelected;
							this->city.tileChanged();
						}
						else {
							std::cout << "Not enough funds\n";
						}
					}
					this->guiSystem.at("selectionCostText").hide();					
					this->actionState = ActionState::NONE;
					this->city.map.clearSelected();
				}
			}
			break;
		}
		/* Zoom the view */
		case sf::Event::MouseWheelMoved:
		{
			if(event.mouseWheel.delta < 0)
			{
				gameView.zoom(1.1f);
				zoomLevel *= 1.1f;
			}
			else
			{
				gameView.zoom(0.9f);
				zoomLevel *= 0.9f;
			}
			//std::cout << "zoomLevel: " << zoomLevel;
			break;
		}
		case sf::Event::KeyReleased:
		{
			if (event.key.code == sf::Keyboard::F1) {
				this->city.map.generate(128, 128, this->game->tileAtlas);
				this->city.shuffleTiles();
			}
		}
		default:
			break;
		}		
	return;
}

GameStateEditor::GameStateEditor(Game* game, MenuOption choice, std::string name)
{
	this->game = game;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	this->guiView.setSize(pos);
	this->gameView.setSize(pos);
	pos *= 0.5f;
	this->guiView.setCenter(pos);
	this->gameView.setCenter(pos);
	
	//map = Map("city_map.dat", 64, 64, game->tileAtlas);

	this->city = City("city", this->game->tileSize, this->game->tileAtlas, choice);
	this->city.shuffleTiles();

	this->zoomLevel = 1.0f;

	/* Centre the camera on the map */
	sf::Vector2f centre(this->city.map.width, this->city.map.height*0.5);
	centre *= float(this->city.map.tileSize);
	gameView.setCenter(centre);

	this->selectionStart = sf::Vector2i(0, 0);
	this->selectionEnd = sf::Vector2i(0, 0);
	this->currentTile = &this->game->tileAtlas.at("grass");	
	this->actionState = ActionState::NONE;

	/* Create GUI Elements */
	this->guiSystem.emplace("rightClickMenu", Gui(sf::Vector2f(196, 16), 2, false, this->game->stylesheets.at("button"),
	{
		std::make_pair("Flatten $" + this->game->tileAtlas["grass"].getCost(), "grass"),
		std::make_pair("Forest $" + this->game->tileAtlas["forest"].getCost(), "forest"),
		std::make_pair("Residential Zone $" + this->game->tileAtlas["residential"].getCost(), "residential"),
		std::make_pair("Commercial Zone $" + this->game->tileAtlas["commercial"].getCost(), "commercial"),
		std::make_pair("Industrial Zone $" + this->game->tileAtlas["industrial"].getCost(), "industrial"),
		std::make_pair("Road $" + this->game->tileAtlas["road"].getCost(), "road"),
		std::make_pair("Fire!", "fire")
	}));

	this->guiSystem.emplace("selectionCostText", Gui(sf::Vector2f(196, 16), 0, false, this->game->stylesheets.at("text"),
	{ std::make_pair("", "") }));

	this->guiSystem.emplace("infoBar", Gui(sf::Vector2f(this->game->window.getSize().x / 5, 16), 2, true, this->game->stylesheets.at("button"),
	{
		std::make_pair("time",          "time"),
		std::make_pair("funds",         "funds"),
		std::make_pair("population",    "population"),
		std::make_pair("employment",    "employment"),
		std::make_pair("current tile",  "tile")
	}));
	this->guiSystem.at("infoBar").setPosition(sf::Vector2f(0, this->game->window.getSize().y - 16));
	this->guiSystem.at("infoBar").show();
	
	this->guiSystem.emplace("tileInfo", Gui(sf::Vector2f(100, 16), 0, false, this->game->stylesheets.at("tileInfo"),
	{ 
		std::make_pair("", ""),
		std::make_pair("", ""),
		std::make_pair("", ""),
		std::make_pair("", ""),
		std::make_pair("", ""),
		std::make_pair("", "")
	}));
}