#include "Overlay.h"

Overlay::Overlay(SDL_DisplayMode* newDisplayMode) {

	//References
	displayMode = newDisplayMode;
	renderer = nullptr;

	//initial values
	blackHoleTexture = nullptr;


}

void Overlay::init(SDL_Renderer* newRenderer, ParticleEngine* pEngine) { //Give renderer once renderer is set up.
	//References
	renderer = newRenderer;
	particleEngine = pEngine;


	//Stuff in overlay init.
	blackHoleTexture = IMG_LoadTexture(renderer, "Eclipse.png");
	std::cout << "ERROR: " << IMG_GetError() << std::endl;



	//Testing
	stars.push_back(Star(0, particleEngine, renderer, displayMode));
	//
}

void Overlay::render() {

	//std::cout << "Called render\n";

	for (int i = 0; i < stars.size(); i++) {
		stars.at(i).render();
	}


	//Rendering random things.
	SDL_Rect eclipseRect;
	eclipseRect.x = (displayMode->w / 2) - (150.0 / 2.0);
	eclipseRect.y = (displayMode->h / 2) - (150.0 / 2.0);
	eclipseRect.h = 150;
	eclipseRect.w = 150;
	SDL_RenderCopy(renderer, blackHoleTexture, NULL, &eclipseRect);


	

}

void Overlay::update(std::chrono::duration<double> delta) {

	for (int i = 0; i < stars.size(); i++) {
		stars.at(i).update(delta);
	}

}
