#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "AnimationHandler.h"
#include <sstream>

enum class TileType { VOID, GRASS, FOREST, WATER, RESIDENTIAL, COMMERCIAL, INDUSTRIAL, ROAD, BRIDGE, FENCE, FIRE };
std::string tileTypeToStr(TileType type);

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str();
    }
}

class Tile
{
public:
	AnimationHandler animHandler;
	sf::Sprite sprite;
	TileType tileType;
	int tileVariant;
	unsigned int regions[1];

	unsigned int cost;
	double population;
	unsigned int maxPopPerLevel;
	unsigned int maxLevels;
	float production;
	float storedGoods;

	Tile() {}

	Tile(const unsigned int tileSize, const unsigned int height, sf::Texture& texture,
		const std::vector<Animation>& animations, const TileType tileType, const unsigned int cost,
		const unsigned int maxPopPerLevel, const unsigned int maxLevels)
	{
		this->tileType = tileType;
		this->tileVariant = 0;
		this->regions[0] = 0;

		this->cost = cost;
		this->population = 0;
		this->maxPopPerLevel = maxPopPerLevel;
		this->maxLevels = maxLevels;
		this->production = 0;
		this->storedGoods = 0;

		this->sprite.setOrigin(sf::Vector2f(0.0f, tileSize*(height - 1)));
		this->sprite.setTexture(texture);
		this->animHandler.frameSize = sf::IntRect(0, 0, tileSize * 2, tileSize*height);
		
		for (auto animation : animations)
		{
			this->animHandler.addAnim(animation);
		}
		this->animHandler.update(0.0f);
	}

	void draw(sf::RenderWindow& window, float dt);
	void update();

	std::string getCost()
	{
		return patch::to_string(this->cost);
	}
};

