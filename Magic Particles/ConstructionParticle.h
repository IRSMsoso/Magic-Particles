#pragma once
#include "Particle.h"
class ConstructionParticle: public Particle {

public:
	ConstructionParticle(POINT* basePosition, int offsetX, int offsetY, SDL_DisplayMode* displayMode, Uint32* color, int size);
	virtual ~ConstructionParticle();

	virtual void update(std::chrono::duration<double> delta);

	int getSize() { return size; }

	Uint32* getColor() { return color; }

	bool getIsConstructed() { return isConstructed; }

private:
	POINT* basePosition;
	int offsetX, offsetY;
	Uint32* color;
	int size;
	bool isConstructed;
};

