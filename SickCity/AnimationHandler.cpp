#include "stdafx.h"
#include "AnimationHandler.h"
#include <vector>

void AnimationHandler::update(const float dt)
{
	if (currentAnim >= this->animations.size() || currentAnim < 0) return;
	
	float duration = this->animations[currentAnim].duration;

	// check iof animation has progressed to a new frame and if so, change to next frame
	if (int((t + dt) / duration) > int(t / duration))
	{
		// calculate frame number
		int frame = int((t + dt) / duration);
		// adjust for looping
		frame %= this->animations[currentAnim].getLength();
		// set sprite to new frame
		sf::IntRect rect = this->frameSize;
		rect.left = rect.width * frame;
		rect.top = (rect.height * this->currentAnim); // -1?
		this->bounds = rect;
	}

	// increment time
	this->t += dt;
	if (this->t > duration * this->animations[currentAnim].getLength()) {
		this->t = 0.0f;
	}
	return;
}

void AnimationHandler::addAnim(Animation& anim)
{
	this->animations.push_back(anim);
}
void AnimationHandler::changeAnim(unsigned int animId)
{
	// do not change animation, if it is currently active or does not exist
	if (this->currentAnim == animId || animId >= this->animations.size()
		|| animId < 0) return;
	// set correct animation
	this->currentAnim = animId;
	// update bounds
	sf::IntRect rect = this->frameSize;
	rect.top = (rect.height * animId);
	this->bounds = rect;
	this->t = 0.0f;
	return;
}