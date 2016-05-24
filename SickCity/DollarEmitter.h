#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Particles/ParticleSystem.hpp>
#include <Thor/Particles/EmissionInterface.hpp>
#include <Thor/Vectors/PolarVector2.hpp>
#include <Thor/Math/Random.hpp>



/*
Essentially, adding an emitter into the particle system is actually making a copy of that emitter. This section in the tutorial introduces refEmitter(), which adds it by reference instead. So, instead of doing:

particle_system.addEmitter(emitter);

I just needed to do:

particle_system.addEmitter(thor::refEmitter(emitter));



GameObject obj;
auto positionFunctor = [&obj] () // capture by reference
{
return obj.getPosition();
};

thor::UniversalEmitter emitter;
emitter.setParticlePosition(positionFunctor);



*/

class DollarEmitter
{
public:
	 DollarEmitter(sf::Vector2f position)
		: mAccumulatedTime(sf::Time::Zero)
		, mPosition(position)
	{
	}

	void operator() (thor::EmissionInterface& system, sf::Vector2f pos)
	{
		
		//const sf::Time tailInterval = explosionDuration / tailsPerExplosion;

		// Accumulate passed time. If enough time has passed (tailInterval), emit a new tail and decrease accumulator.
		/*mAccumulatedTime += dt;
		while (mAccumulatedTime - tailInterval > sf::Time::Zero)
		{
			emitTail(system);
			mAccumulatedTime -= tailInterval;
		}
		*/
		emitDollar(system, pos);
	}
	void emitDollar(thor::EmissionInterface& system, sf::Vector2f pos) {
		thor::Particle particle(sf::seconds(1.5f));
		thor::PolarVector2f velocity(5, 90);
		particle.position = pos;
		particle.velocity = velocity;
		system.emitParticle(particle);
	}
private:
	sf::Time		mAccumulatedTime;
	sf::Vector2f	mPosition;
	sf::Color		mColor;
};