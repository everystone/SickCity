#include "stdafx.h"
#include "Player.h"

const float PI = 3.14159265;
void Player::setPath(std::vector<Tile*> path)
{
	this->path = path;
	if (path.size() > 0) {
		this->factor = .3f;
		this->pathStep = 0;
		this->destination = path[pathStep]->getPosition();
		//destination.x += 16;
		//destination.y += 8;
		this->state = PlayerState::Moving;
		this->animator.playAnimation("walk", true);
		std::cout << "State : Moving" << std::endl;
	}
	else {
		this->state = PlayerState::Idle;
		this->animator.stopAnimation();
	}
}

sf::Vector2f Player::getPosition()
{
	sf::Vector2f pos = this->sprite.getPosition();
	//pos += this->sprite.getOrigin();
	return pos;
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

	if (this->state == PlayerState::Moving && this->path.size() > 0) {
		
		//std::cout << "Dest: " << destination.x << "," << destination.y << std::endl;
		if (sprite.getPosition() != destination) {			
			// move sprite
			//sf::Vector2f direction = destination - sprite.getPosition();			
			//float magnitude = sqrt((direction.x * direction.x) + (direction.y * direction.y));
			//sf::Vector2f unitVector(direction.x / magnitude, direction.y / magnitude);
			//sprite.move(unitVector * moveSpeed * dt);

			//sf::Vector2f velocity = (destination - sprite.getPosition());
			//sprite.move(velocity * dt);
			factor += (dt * moveSpeed);
			sprite.setPosition(Interpolate(getPosition(), destination, factor));
		}
		else if (pathStep < path.size()-1) {
			pathStep++;
			destination = path[pathStep]->getPosition();
			//destination.x += 16;
			//destination.y += 8;
			this->factor = .3f;
			std::cout << "step: " << pathStep << std::endl;
			// rotate towards destination
			float dx = destination.x - this->getPosition().x;
			float dy = destination.y - this->getPosition().y;
			this->rotation = (atan2(dy, dx)) * 180 / PI;
			this->rotation += 90.0f;
			std::cout << "rotation: " << rotation << std::endl;
			sprite.setRotation(rotation);
		}else {
			this->state = PlayerState::Idle;
			animator.stopAnimation();
			std::cout << "State : Idle" << std::endl;
		}
	}
}


Player::Player(sf::Texture& texture, sf::Vector2f pos)
{
	this->sprite.setTexture(texture);
	this->sprite.setPosition(pos);
}

/*
void Player::rotate(sf::Transformable& animated, float progress)
{
	animated.setRotation(360.f * progress);
}*/

void Player::Spawn(sf::Vector2f pos)
{
	this->sprite.setPosition(pos);
	this->sprite.setOrigin(sf::Vector2f(8, 8));	
	this->health = 100;

	// setup animations
	thor::FrameAnimation walk, idle, rotate;
	walk.addFrame(1.0f, sf::IntRect(0, 0, 16, 16));
	walk.addFrame(1.0f, sf::IntRect(16, 0, 16, 16));
	animator.addAnimation("walk", walk, sf::seconds(2.0f));
	//animator.addAnimation("rotate", &rotate, sf::seconds(1.f));
	animator.playAnimation("walk", false);


}
