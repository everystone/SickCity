#pragma once
#include <SFML\Graphics.hpp>
#include <Thor\Animations.hpp>
#include "Tile.h"

/*
	Player class
	Movement: uses A* pathfinding to move to clicked Tiles.
	Bulding on tiles is possible in a radious of X meters around player.
	Can not walk on water
	Can get into Vehicles ( boats )
	Has health
	
*/

class Player {
private:
	enum class PlayerState { Idle, Moving, Building };
	int health = 100;
	thor::Animator<sf::Sprite, std::string> animator;

	float moveSpeed = .2f;
	float factor = .1f;
	int pathStep = 0;
	PlayerState state = PlayerState::Idle;
	std::vector<Tile*> path;
public:
	void setPath(std::vector<Tile*> path);
	sf::Sprite sprite;
	void draw(sf::RenderWindow& window, float dt);
	void update(float dt);
	void setDestination(sf::Vector2f pos);
	Player(){	}
	Player(sf::Texture& texture, sf::Vector2f pos);
	void Spawn(sf::Vector2f pos);
};