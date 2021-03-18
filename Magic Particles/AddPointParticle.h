#pragma once
#include "Particle.h"


class AddPointParticle: public Particle {


public:
	AddPointParticle(MathVector point);

	virtual void update(std::chrono::duration<double> delta, ParticleEngine* pEngine);

};

