#include "ParticleEngine.h"





ParticleEngine::ParticleEngine(SDL_DisplayMode* newDisplayMode) {
	addPointParticleTexture = nullptr;
	losePointParticleTexture = nullptr;
	pointDeleteCount = 0;
	displayMode = newDisplayMode;
}

void ParticleEngine::update(std::chrono::duration<double> delta, SDL_DisplayMode* displayMode, bool shouldUpdateOverlayParticles) {
	GetCursorPos(&mousePos);

	for (int i = 0; i < particles.size(); i++) {
		//Call update here.


		if (particles.at(i)->getParticleType() != ParticleType::ConstructionParticleType) {  //Construction particles are updated by their stars.
			particles.at(i)->update(delta);
		}


		//Delete particles that should be deleted.
		if (particles.at(i)->getShouldDelete()) {

			if (particles.at(i)->getParticleType() == ParticleType::AddPointParticleType) {
				incrementPointDeleteCount();
			}
			

			delete particles.at(i);
			particles.erase(particles.begin() + i);
			i--;
		}
	}

	
}

void ParticleEngine::render(SDL_Renderer* renderer, bool shouldShowOverlayParticles) {

	for (int i = 0; i < particles.size(); i++) {

		ParticleType particleType = particles.at(i)->getParticleType();
		if (particleType == ParticleType::PixelParticleType) {
			PixelParticle* particle = dynamic_cast<PixelParticle*>(particles.at(i));
			Uint8* colors = (Uint8*)particle->getColor();
			SDL_SetRenderDrawColor(renderer, colors[2], colors[1], colors[0], 255);
			SDL_Rect rect;
			rect.w = particle->getSize();
			rect.h = particle->getSize();
			rect.x = particle->getPosition().x - rect.w / 2;
			rect.y = particle->getPosition().y - rect.h / 2;
			SDL_RenderFillRect(renderer, &rect);

			//std::cout << "Rendered pixel particle at: " << particle->getPosition().x << ", " << particle->getPosition().y << std::endl;
		}
		else if (shouldShowOverlayParticles && particleType == ParticleType::ConstructionParticleType) {
			ConstructionParticle* particle = dynamic_cast<ConstructionParticle*>(particles.at(i));
			Uint8* colors = (Uint8*)particle->getColor();
			SDL_SetRenderDrawColor(renderer, colors[0], colors[1], colors[2], 255);
			SDL_Rect rect;
			rect.w = particle->getSize();
			rect.h = particle->getSize();
			rect.x = particle->getPosition().x - rect.w / 2;
			rect.y = particle->getPosition().y - rect.h / 2;
			SDL_RenderFillRect(renderer, &rect);
		}
		else {
			SDL_Point point = particles.at(i)->getNearestIntPoint();
			//SDL_RenderDrawPoint(renderer, point.x, point.y);
			SDL_Rect rect;
			SDL_Texture* textureToUse = nullptr;
			if (particles.at(i)->getParticleType() == ParticleType::AddPointParticleType) {
				textureToUse = addPointParticleTexture;
			}
			if (particles.at(i)->getParticleType() == ParticleType::LosePointParticleType) {
				textureToUse = losePointParticleTexture;
			}
			if (textureToUse != nullptr) {
				SDL_QueryTexture(textureToUse, NULL, NULL, &rect.w, &rect.h);
				rect.x = point.x - (rect.w) / 2;
				rect.y = point.y - (rect.h) / 2;
				SDL_RenderCopy(renderer, textureToUse, NULL, &rect);
			}
		}
	}
}

void ParticleEngine::spawnParticle(Particle* particle) {
	particles.push_back(particle);
}

void ParticleEngine::init(SDL_Renderer* renderer) {
	addPointParticleTexture = IMG_LoadTexture(renderer, "magicparticle1.png");
	losePointParticleTexture = IMG_LoadTexture(renderer, "magicparticle2.png");
	if (addPointParticleTexture == NULL || losePointParticleTexture == NULL) {
		printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
	}
}

unsigned int ParticleEngine::getDeleteCount()
{
	unsigned int returnVal = pointDeleteCount;
	pointDeleteCount = 0;
	return returnVal;
}

bool ParticleEngine::hasParticlesToRender()
{
	return (particles.size() > 0);
}


unsigned int ParticleEngine::getPointParticleCount()
{
	unsigned int count = 0;

	for (int i = 0; i < particles.size(); i++) {
		if (particles.at(i)->getParticleType() == ParticleType::AddPointParticleType)
			count++;
	}
	return count;
}

void ParticleEngine::clearParticlesOfType(ParticleType type) {

	for (int i = 0; i < particles.size(); i++) {
		if (particles.at(i)->getParticleType() == type) {
			delete particles.at(i);
			particles.erase(particles.begin() + i);
			i--;
		}
	}

}

void ParticleEngine::deleteParticle(Particle* particle) {
	for (int i = 0; i < particles.size(); i++) {
		if (particles.at(i) == particle) {
			delete particles.at(i);
			particles.erase(particles.begin() + i);
			i--;
		}
	}
}

