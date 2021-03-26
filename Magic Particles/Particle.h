#pragma once
#include <SDL.h>
#include "MathVector.h"
#include <chrono>
#include <iostream>
#include <Windows.h>


enum ParticleType {
	ParticleErrorType,
	AddPointParticleType,
	LosePointParticleType,
	PixelParticleType,
	ConstructionParticleType,
};


class Particle {


public:
	Particle(MathVector point, POINT* mousePosition, SDL_DisplayMode* displayMode);

	virtual ~Particle();

	void setPosition(MathVector);
	void setVelocity(MathVector);
	void setAcceleration(MathVector);

	SDL_Point getNearestIntPoint();
	MathVector getPosition() { return position; }
	MathVector getVelocity() { return velocity; }
	bool getShouldDelete() { return shouldDie; }
	ParticleType getParticleType() { return particleType; }

	virtual void update(std::chrono::duration<double> delta); //Particles should override this if needed, being sure to call super() for acc -> vel and vel -> pos logic.


protected:
	MathVector position; //Pixels
	MathVector velocity; //Pixels / Second
	MathVector acceleration; //Pixels / Second / Second


	POINT* mousePosition;
	SDL_DisplayMode* displayMode;

	
	ParticleType particleType;
	bool shouldDie;

	//Dying of old age logic members.
	std::chrono::system_clock::time_point birthTime;
	std::chrono::duration<double> lifeSpan;

	int velocityCap = 400; //Default
};

