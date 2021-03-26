#include "Star.h"

Star::Star(unsigned int newPoints, ParticleEngine* pEngine, SDL_Renderer* newRenderer, SDL_DisplayMode* newDisplayMode) {
	points = newPoints;
	constructed = false;
	particleEngine = pEngine;
	renderer = newRenderer;
	displayMode = newDisplayMode;

	starTexture = nullptr;

	//Testing

	position = MathVector(100, 100);
	velocity = MathVector(0, 0);
	acceleration = MathVector(0, 0);

	sharedPosition = new POINT;

	//

	rebuild();

}

void Star::update(std::chrono::duration<double> delta) {

	//Physics behaviors.







	//POSITION STUFF
	velocity.x += acceleration.x;
	velocity.y += acceleration.y;
	position.x += velocity.x;
	position.y += velocity.y;
	std::cout << "Star Position: " << position.x << ", " << position.y << std::endl;
	std::cout << "Star velocity: " << velocity.x << ", " << velocity.y << std::endl;
	std::cout << "Star acceleration: " << acceleration.x << ", " << acceleration.y << std::endl;
	
	sharedPosition->x = position.x - sizeX / 2.0;
	sharedPosition->y = position.y - sizeY / 2.0;





	//Update the star's particles
	for (int i = 0; i < buildingParticles.size(); i++) {
		buildingParticles.at(i)->update(delta);
	}


	if (!constructed) {

		bool canBeConstructed = true;

		for (int i = 0; i < buildingParticles.size(); i++) {
			if (!buildingParticles.at(i)->getIsConstructed()) {
				canBeConstructed = false;
				break;
			}
		}

		if (canBeConstructed) {
			clearParticles();
			constructed = true;
			std::cout << "Constructed Star!\n";
		}

	}

}

void Star::render() {

	if (constructed) {
		SDL_Rect rect;
		rect.x = position.x - sizeX / 2.0;
		rect.y = position.y - sizeY / 2.0;
		rect.w = sizeX;
		rect.h = sizeY;
		std::cout << "Rect: " << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << "\n";
		SDL_RenderCopy(renderer, starTexture, NULL, &rect);
	}

}

void Star::rebuild() {

	starTexture = IMG_LoadTexture(renderer, "Star.png"); //Change texture based on points.

	clearParticles(); //Clear any straggling particles.

	if (constructed) { //If this is true we have to spawn deconstruction particles and set non constructed so that it doesn't render in full.
		//code for spawning particles
		constructed = false;
	}

	SDL_Surface* starSurface = IMG_Load("Star.png");
	
	SDL_QueryTexture(starTexture, nullptr, nullptr, &sizeX, &sizeY);

	int bpp = starSurface->format->BytesPerPixel;

	for (int y = 0; y < starSurface->h; y++) {
		for (int x = 0; x < starSurface->w; x++) {
			Uint32* color = new Uint32;
			*color = *(Uint32*)((Uint8*)starSurface->pixels + y * starSurface->pitch + x * bpp);

			int alpha = ((Uint8*)color)[3];
			//std::cout << "Alpha Value: " << alpha << std::endl;
			if (alpha != 0) {
				ConstructionParticle* particle = new ConstructionParticle(sharedPosition, x, y, displayMode, color, 1);
				particle->setVelocity(MathVector((rand() % 100) - 50, (rand() % 100) - 50));
				buildingParticles.push_back(particle);
				particleEngine->spawnParticle(particle);
			}
		}
	}
}

void Star::clearParticles() {
	std::cout << "Clear All Particles Called\n";


	while (buildingParticles.size() > 0) {
		particleEngine->deleteParticle(buildingParticles.at(0));
		buildingParticles.erase(buildingParticles.begin());
	}
}
