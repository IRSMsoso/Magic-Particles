#pragma once
#include "Particle.h"


class PixelParticle: public Particle {

public:

	PixelParticle(MathVector point, POINT* mousePosition, SDL_DisplayMode* displaymode, Uint32* color, int size);

	virtual ~PixelParticle();

	virtual void update(std::chrono::duration<double> delta);

	int getSize() { return size; }


	Uint32* getColor() { return color; }

private:

	Uint32* color;

	int size;

};

