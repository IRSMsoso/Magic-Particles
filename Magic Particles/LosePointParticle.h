#pragma once
#include "Particle.h"
class LosePointParticle: public Particle {

public:

	LosePointParticle(MathVector point);

	virtual void update(std::chrono::duration<double> delta, ParticleEngine* pEngine);


};

