#pragma once
#include <SDL.h>
#include "MathVector.h"
#include <chrono>
#include <iostream>


const static int CAPPEDVELOCITY = 400;

class Particle {


public:
	Particle(SDL_Point point);

	void setPosition(MathVector);
	void setVelocity(MathVector);
	void setAcceleration(MathVector);

	SDL_Point getNearestIntPoint();
	MathVector getPosition() { return position; }
	MathVector getVelocity() { return velocity; }

	void update(std::chrono::duration<double> delta);


private:
	MathVector position; //Pixels
	MathVector velocity; //Pixels / Second
	MathVector acceleration; //Pixels / Second / Second

};

