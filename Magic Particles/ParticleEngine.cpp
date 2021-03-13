#include "ParticleEngine.h"

ParticleEngine::ParticleEngine() {
	particleTexture = NULL;
}

void ParticleEngine::update(std::chrono::duration<double> delta, SDL_DisplayMode* displayMode) {
	POINT p;
	GetCursorPos(&p);
	for (int i = 0; i < particles.size(); i++) {
		//Set Acceleration.
		//std::cout << "Mouse: " << p.x << ", " << p.y << std::endl;
		MathVector diffVector = MathVector(p.x + 2, p.y + 7) - particles.at(i)->getPosition();


		////Doesn't Seem like this top one is neccessary to get the desired effect.
		//if (diffVector.getMagnitude() < 0) { 
			//MathVector velVector = diffVector.getUnitVector() * max(pow(diffVector.getMagnitude(), 2), 0);
			//std::cout << "Difference Vector: " << diffVector.x << ", " << diffVector.y << std::endl;

			//particles.at(i)->setVelocity(velVector);

			//particles.at(i)->update(delta);
		//}
		//else {
		MathVector accVector = (diffVector.getUnitVector() / pow(diffVector.getMagnitude(), 2)) * 100000000;
		//std::cout << "Difference Vector: " << diffVector.x << ", " << diffVector.y << std::endl;

		particles.at(i)->setAcceleration(accVector);

		particles.at(i)->update(delta);
		//}

		MathVector particlePosition = particles.at(i)->getPosition();
		MathVector particleVelocity = particles.at(i)->getVelocity();

		if (particlePosition.x < 0)
			particles.at(i)->setVelocity(MathVector(abs(particleVelocity.x), particleVelocity.y));
		else if (particlePosition.x > displayMode->w)
			particles.at(i)->setVelocity(MathVector(abs(particleVelocity.x) * -1, particleVelocity.y));
		if (particlePosition.y < 0)
			particles.at(i)->setVelocity(MathVector(particleVelocity.x, abs(particleVelocity.y)));
		else if (particlePosition.y > displayMode->h)
			particles.at(i)->setVelocity(MathVector(particleVelocity.x, abs(particleVelocity.y) * -1));

		//Delete the ones that are close.
		if (diffVector.getMagnitude() < 3) {
			delete particles.at(i);
			particles.erase(particles.begin() + i);
			i--;
			//std::cout << "Deleting Particle...\n";
			//std::cout << "Size: " << particles.size() << std::endl;
		}
	}
	std::cout << "Particle List Length: " << particles.size() << std::endl;
}

void ParticleEngine::render(SDL_Renderer* renderer) {
	for (int i = 0; i < particles.size(); i++) {
		SDL_Point point = particles.at(i)->getNearestIntPoint();
		SDL_Color newColor = particles.at(i)->getColor();
		SDL_SetRenderDrawColor(renderer, newColor.r, newColor.g, newColor.b, newColor.a);
		//SDL_RenderDrawPoint(renderer, point.x, point.y);
		SDL_Rect rect;
		rect.x = point.x;
		rect.y = point.y;
		rect.w = 6;
		rect.h = 6;
		SDL_RenderCopy(renderer, particleTexture, NULL, &rect);
		//SDL_RenderFillRect(renderer, &rect);

	}
}

void ParticleEngine::spawnParticle(SDL_Point point, SDL_Color color) {
	particles.push_back(new Particle(point, color));
}

void ParticleEngine::init(SDL_Renderer* renderer) {
	particleTexture = IMG_LoadTexture(renderer, "magicparticle1.png");
	if (particleTexture == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", "magicparticle1.png", IMG_GetError());
	}
}