#include "stdafx.h"
#include "Player.h"

void Player::draw(sf::RenderWindow& window, float dt)
{
	// draw player
	window.draw(this->sprite);
}

void Player::update(float dt)
{
	animator.update(sf::seconds(dt));
	animator.animate(sprite);
}

void Player::moveTo(sf::Vector2f pos)
{
}

sf::Vector2f Player::getPos()
{
	return this->sprite.getPosition();
}

Player::Player(sf::Texture& texture, sf::Vector2f pos)
{
	this->sprite.setTexture(texture);
	this->sprite.setPosition(pos);
	

}

void Player::Spawn(sf::Vector2f pos)
{
	this->sprite.setPosition(pos);
	this->health = 100;

	// setup animations
	thor::FrameAnimation walk, idle;
	walk.addFrame(1.0f, sf::IntRect(0, 0, 16, 16));
	walk.addFrame(1.0f, sf::IntRect(16, 0, 16, 16));
	animator.addAnimation("walk", walk, sf::seconds(2.0f));
	animator.playAnimation("walk", true);
}
