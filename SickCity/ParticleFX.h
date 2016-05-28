#pragma once
#include "TextureManager.h"
#include <Thor/Particles.hpp>
#include <Thor/Math/Distributions.hpp>
#include <Thor/Vectors/PolarVector2.hpp>
#include <Thor/Animations.hpp>
#include <Thor/Math/Random.hpp>

class ParticleFX {
private:
	TextureManager* texmgr;
	const int tileSize;

public:
	thor::ParticleSystem weatherSystem;
	thor::ParticleSystem particleSystem;

	void spawnRain();
	void operator()(TextureManager tex);
	void emitParticle(unsigned int index, sf::Vector2f pos, float scale);
	ParticleFX(TextureManager tex, const int tileSize);
	ParticleFX() : tileSize(16) {};
};