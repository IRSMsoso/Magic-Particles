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

	void spawnParticle(SDL_Point point, SDL_Color color);

	void init(SDL_Renderer* renderer);

private:
	std::vector<Particle*> particles;
	std::vector<Particle*> particleTrash;

	SDL_Texture* particleTexture;

};
