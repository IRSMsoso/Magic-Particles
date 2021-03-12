#pragma once
#include <SDL.h>
#include "MathVector.h"
#include <chrono>
#include <iostream>


const static int CAPPEDVELOCITY = 300;

class Particle {


public:
	Particle(SDL_Point point, SDL_Color color);

	void setPosition(MathVector);
	void setVelocity(MathVector);
	void setAcceleration(MathVector);

	SDL_Point getNearestIntPoint();
	SDL_Color getColor() { return color; }
	MathVector getPosition() { return position; }
	MathVector getVelocity() { return velocity; }

	void update(std::chrono::duration<double> delta);


private:
	SDL_Color color;
	MathVector position; //Pixels
	MathVector velocity; //Pixels / Second
	MathVector acceleration; //Pixels / Second / Second

};

