#pragma once
#include <vector>
#include "ConstructionParticle.h"
#include "ParticleEngine.h"

class Star {

public:
	Star(unsigned int points, ParticleEngine* pEngine, SDL_Renderer* renderer, SDL_DisplayMode* displayMode);

	void update(std::chrono::duration<double> delta);
	void render();

	void rebuild();

	bool isConstructed() { return constructed; }
private:
	

	unsigned int points;
	bool constructed;
	SDL_Renderer* renderer;

	SDL_Texture* starTexture;

	MathVector position;
	MathVector velocity;
	MathVector acceleration;
	int sizeX;
	int sizeY;

	POINT* sharedPosition; //Not sure what's up with why this is necessary.


	void clearParticles();
	std::vector<ConstructionParticle*> buildingParticles;
	
	//References
	ParticleEngine* particleEngine;
	SDL_DisplayMode* displayMode;

};

