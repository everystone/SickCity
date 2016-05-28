#include "stdafx.h"
#include "ParticleFX.h"


ParticleFX::ParticleFX(TextureManager tex, int tileSize): texmgr(&tex),tileSize(tileSize) {

	// Setup Particle Systems
	thor::FadeAnimation fader(.2f, 0.2f);
	particleSystem.addAffector(thor::AnimationAffector(fader));
	this->particleSystem.setTexture(this->texmgr->getRef("dollar"));
}
void ParticleFX::spawnRain()
{
	// Weather
	thor::FadeAnimation fader(.2f, 0.2f);
	particleSystem.addAffector(thor::AnimationAffector(fader));
	this->weatherSystem.setTexture(this->texmgr->getRef("particle"));
	//this->weatherSystem.addAffector(thor::TorqueAffector(50.f));
	//this->weatherSystem.addAffector(thor::ForceAffector(sf::Vector2f(10.f, 10.f)));
	this->weatherSystem.addAffector(thor::AnimationAffector(fader));
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 3; x++) {
			thor::UniversalEmitter rainEmitter;

			rainEmitter.setParticleColor(sf::Color(0, 100, 150, 50));
			rainEmitter.setParticleScale(sf::Vector2f(.3f, .3f));
			//rainEmitter.setParticlePosition();
			rainEmitter.setEmissionRate(thor::random(0.2f, 0.5f));
			thor::PolarVector2f velocity(thor::random(5.0f, 20.0f), 75.0f);
			rainEmitter.setParticleLifetime(sf::seconds(2));
			rainEmitter.setParticleVelocity(velocity);
			sf::Vector2f pos;
			pos.x = (x - y) * this->tileSize + 64 * this->tileSize;
			pos.y = (x + y) * this->tileSize * 0.5;
			rainEmitter.setParticlePosition(pos);
			this->weatherSystem.addEmitter(rainEmitter);
		}
	}
}

void ParticleFX::operator()(TextureManager tex)
{
	this->texmgr = &tex;
	//thor::FadeAnimation fader(.2f, 0.2f);
	//particleSystem.addAffector(thor::AnimationAffector(fader));
	this->particleSystem.setTexture(this->texmgr->getRef("dollar"));
	this->weatherSystem.setTexture(this->texmgr->getRef("particle"));
	
	//spawnRain();
	std::cout << "particleFx init";
}

void ParticleFX::emitParticle(unsigned int index, sf::Vector2f pos, float scale)
{
	thor::UniversalEmitter emitter;

	emitter.setParticlePosition(pos);
	thor::PolarVector2f velocity(thor::random(5.0f, 20.0f), 245.0f); // thor::random(230.0f, 320.0f)																	 //emitter.setParticleVelocity(thor::Distributions::deflect({ 0,-25 }, 10.0f));
	emitter.setParticleVelocity(velocity);
	//emitter.setEmissionRate();
	emitter.setParticleLifetime(sf::seconds(thor::random(1.0f, 1.5f)));
	//emitter.setParticleScale(sf::Vector2f(scale, scale));
	//emitter.setParticleScale(sf::Vector2f(.2f, .2f));
	//emitter.setParticleTextureInde(2);
	emitter.setParticleColor(sf::Color(100, 255, 135));
	this->particleSystem.addEmitter(emitter, sf::seconds(1));
}

