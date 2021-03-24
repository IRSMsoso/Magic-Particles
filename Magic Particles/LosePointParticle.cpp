#include "LosePointParticle.h"

LosePointParticle::LosePointParticle(MathVector point, POINT* newMousePosition): Particle(point, newMousePosition, nullptr) {
	velocityCap = 400;
	particleType = ParticleType::LosePointParticleType;
	
	lifeSpan = std::chrono::seconds((rand() & 3) + 2);
}

void LosePointParticle::update(std::chrono::duration<double> delta) {

	Particle::update(delta);

	acceleration = velocity * -0.6;


}
