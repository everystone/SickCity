#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Animation
{
public:
	unsigned int startFrame;
	unsigned int endFrame;
	float duration;

	// Constructor
	Animation(unsigned int startFrame, unsigned int endFrame, float duration)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->duration = duration;
	}
	unsigned int getLength() {
		return endFrame - startFrame + 1; 
	}
};

class AnimationHandler
{
private:
	std::vector<Animation> animations;

	// current time since the animation loop started
	float t;

	int currentAnim;
public:
	void addAnim(Animation& anim);
	void update(const float dt);
	void changeAnim(unsigned int animNum);
	sf::IntRect bounds;
	sf::IntRect frameSize;

	// Constructor
	AnimationHandler() {
		this->t = 0.0f;
		this->currentAnim = -1;
	}
	AnimationHandler(const sf::IntRect& frameSize)
	{
		this->frameSize = frameSize;
		this->t = 0.0f;
		this->currentAnim = -1;
	}
};