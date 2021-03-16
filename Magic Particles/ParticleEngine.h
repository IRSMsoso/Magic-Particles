#pragma once
#include <vector>
#include "Particle.h"
#include <iostream>
#include <Windows.h>
#include <SDL_image.h>

class ParticleEngine {

public:
	ParticleEngine();

	void update(std::chrono::duration<double> delta, SDL_DisplayMode* displayMode);
	
	void render(SDL_Renderer* renderer);

	void spawnParticle(SDL_Point point, SDL_Point velocity);

	void init(SDL_Renderer* renderer);

	unsigned int getDeleteCount();

	bool needsRendering() { return (particles.size() != 0); }

	unsigned int getPointParticleCount() { return particles.size(); }

private:
	std::vector<Particle*> particles;

	unsigned int deleteCount;

	SDL_Texture* particleTexture;

};

