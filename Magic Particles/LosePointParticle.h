#pragma once
#include "Particle.h"
class LosePointParticle: public Particle {

public:

	LosePointParticle(MathVector point, POINT* newMousePosition);

	virtual void update(std::chrono::duration<double> delta);


};

