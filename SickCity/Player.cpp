#include "stdafx.h"
#include "Player.h"

const float PI = 3.14159265;
void Player::setPath(std::vector<Tile*> path)
{
	if (path.size() > 0) {
		this->factor = .3f;
		this->pathStep = 0;
		this->path = path;
		this->state = PlayerState::Moving;
		std::cout << "State : Moving" << std::endl;
	}

}

void Player::draw(sf::RenderWindow& window, float dt)
{
	// draw player
	window.draw(this->sprite);
}

sf::Vector2f Interpolate(
	const sf::Vector2f& pointA,
	const sf::Vector2f& pointB,
	float factor
	) {
	if (factor > 1.f)
		factor = 1.f;

	else if (factor < 0.f)
		factor = 0.f;

	return pointA + (pointB - pointA) * factor;
}

void Player::update(float dt)
{
	animator.update(sf::seconds(dt));
	animator.animate(sprite);

	if (this->state == PlayerState::Moving) {
		sf::Vector2f destination = path[pathStep]->sprite.getPosition();
		//std::cout << "Dest: " << destination.x << "," << destination.y << std::endl;
		if (sprite.getPosition() != destination) {
			// rotate towards destination
			//float dx = sprite.getPosition().x - destination.x;
			//float dy = sprite.getPosition().y - destination.y;
			//float rotation = (atan2(dy, dx)) * 180 / PI;
			//sprite.setRotation(rotation);
			
			// move sprite
			//sf::Vector2f direction = destination - sprite.getPosition();			
			//float magnitude = sqrt((direction.x * direction.x) + (direction.y * direction.y));
			//sf::Vector2f unitVector(direction.x / magnitude, direction.y / magnitude);
			//sprite.move(unitVector * moveSpeed * dt);

			//sf::Vector2f velocity = (destination - sprite.getPosition());
			//sprite.move(velocity * dt);
			factor += (dt * moveSpeed);
			sprite.setPosition(Interpolate(sprite.getPosition(), destination, factor));
		}
		else if (pathStep < path.size()-1) {
			pathStep++;
			this->factor = .3f;
			std::cout << "step: " << pathStep << std::endl;
		}else {
			this->state = PlayerState::Idle;
			std::cout << "State : Idle" << std::endl;
		}
	}
}

void Player::setDestination(sf::Vector2f pos)
{
	//this->destination = pos;
	this->factor = .1f;
	this->state = PlayerState::Moving;
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
