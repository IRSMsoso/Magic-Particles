#pragma once
#include <SDL.h>
#include "MathVector.h"
#include <chrono>
#include <iostream>
#include "ParticleEngine.h"

class ParticleEngine; //Forward declaration of ParticleEngine so that we can use it's pointer to spawn particles.

class Particle {


public:
	Particle(MathVector point);

	virtual ~Particle();

	void setPosition(MathVector);
	void setVelocity(MathVector);
	void setAcceleration(MathVector);

	SDL_Point getNearestIntPoint();
	MathVector getPosition() { return position; }
	MathVector getVelocity() { return velocity; }
	bool getShouldDelete() { return shouldDie; }
	bool getShouldGivePointsOnDeath() { return shouldGivePointsOnDeath; }

	virtual void update(std::chrono::duration<double> delta, ParticleEngine* pEngine); //Particles should override this if needed, being sure to call super() for acc -> vel and vel -> pos logic.


protected:
	MathVector position; //Pixels
	MathVector velocity; //Pixels / Second
	MathVector acceleration; //Pixels / Second / Second

	bool shouldGivePointsOnDeath; // Kind of a hacky way to do it but w/e.
	bool shouldDie;

	int velocityCap = 400; //Default
};

