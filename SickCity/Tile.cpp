#include "stdafx.h"
#include "Tile.h"
#include "AnimationHandler.h"

void Tile::draw(sf::RenderWindow & window, float dt)
{
	// change sprite to reflect tile variant
	this->animHandler.changeAnim(this->tileVariant);

	// update animation
	this->animHandler.update(dt);

	// update sprite
	this->sprite.setTextureRect(this->animHandler.bounds);

	// draw tile
	window.draw(this->sprite);
	return;
}

void Tile::update()
{
	/* If the population is at the maximum value for the tile,
	* there is a small chance that the tile will increase its
	* building stage */
	if ((this->tileType == TileType::RESIDENTIAL ||
		this->tileType == TileType::COMMERCIAL ||
		this->tileType == TileType::INDUSTRIAL) &&
		this->population == this->maxPopPerLevel * (this->tileVariant + 1) &&
		this->tileVariant < this->maxLevels)
	{
		if (rand() % int(1e4) < 1e2 / (this->tileVariant + 1)) {
			++this->tileVariant;
			std::cout << "Tile leveled up: " << tileTypeToStr(this->tileType);
		}
	}

	return;
}

std::string tileTypeToStr(TileType type)
{
	switch (type)
	{
	default:
	case TileType::VOID:            return "Void";
	case TileType::GRASS:           return "Flatten";
	case TileType::FOREST:          return "Forest";
	case TileType::WATER:           return "Water";
	case TileType::RESIDENTIAL:     return "Residential Zone";
	case TileType::COMMERCIAL:      return "Commercial Zone";
	case TileType::INDUSTRIAL:      return "Industrial Zone";
	}
}

