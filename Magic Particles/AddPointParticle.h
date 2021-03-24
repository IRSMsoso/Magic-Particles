#pragma once
#include "Particle.h"


class AddPointParticle: public Particle {


public:
	AddPointParticle(MathVector point, POINT* mousePosition, SDL_DisplayMode* displayMode);

	virtual void update(std::chrono::duration<double> delta);

};

