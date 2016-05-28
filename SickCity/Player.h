#pragma once
#include <SFML\Graphics.hpp>
#include <Thor\Animations.hpp>

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
	int health = 100;
	thor::Animator<sf::Sprite, std::string> animator;

public:
	sf::Sprite sprite;
	void draw(sf::RenderWindow& window, float dt);
	void update(float dt);
	void moveTo(sf::Vector2f pos);
	sf::Vector2f getPos();
	Player(){	}
	Player(sf::Texture& texture, sf::Vector2f pos);
	void Spawn(sf::Vector2f pos);
};