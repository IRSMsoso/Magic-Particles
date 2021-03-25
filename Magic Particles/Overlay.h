#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Overlay {

public:
	Overlay(SDL_DisplayMode* displayMode);

	void init(SDL_Renderer* renderer);

	void render(SDL_Renderer* renderer);
	void update();



private:

	//Stuff On Overlay.
	SDL_Texture* blackHoleTexture;



	//References
	SDL_DisplayMode* displayMode;

};

