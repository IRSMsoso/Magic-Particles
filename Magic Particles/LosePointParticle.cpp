#include "LosePointParticle.h"

LosePointParticle::LosePointParticle(MathVector point): Particle(point) {
	velocityCap = 400;
	particleType = ParticleType::LosePointParticleType;
	
	lifeSpan = std::chrono::seconds((rand() & 3) + 2);
}

void LosePointParticle::update(std::chrono::duration<double> delta, ParticleEngine* pEngine) {

	Particle::update(delta, pEngine);

	acceleration = velocity * -0.6;


}
