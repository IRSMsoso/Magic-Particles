#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Star.h"

class Overlay {

public:
	Overlay(SDL_DisplayMode* displayMode);

	void init(SDL_Renderer* renderer, ParticleEngine* pEngine);

	void render();
	void update(std::chrono::duration<double> delta);



private:

	//Stuff On Overlay.
	SDL_Texture* blackHoleTexture;
	std::vector<Star> stars;


	//References
	SDL_DisplayMode* displayMode;
	ParticleEngine* particleEngine;
	SDL_Renderer* renderer;

};

