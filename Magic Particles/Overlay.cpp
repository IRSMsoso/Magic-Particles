#include "Overlay.h"

Overlay::Overlay(SDL_DisplayMode* newDisplayMode) {



	//initial values
	blackHoleTexture = nullptr;
	displayMode = newDisplayMode;
}

void Overlay::init(SDL_Renderer* renderer) {

	//Stuff in overlay init.
	blackHoleTexture = IMG_LoadTexture(renderer, "Eclipse.png");
	std::cout << "ERROR: " << IMG_GetError() << std::endl;
}

void Overlay::render(SDL_Renderer* renderer) {

	std::cout << "Called render\n";

	//Rendering random things.
	SDL_Rect eclipseRect;
	eclipseRect.x = (displayMode->w / 2) - (150.0 / 2.0);
	eclipseRect.y = (displayMode->h / 2) - (150.0 / 2.0);
	eclipseRect.h = 150;
	eclipseRect.w = 150;
	SDL_RenderCopy(renderer, blackHoleTexture, NULL, &eclipseRect);

}

void Overlay::update()
{
}
