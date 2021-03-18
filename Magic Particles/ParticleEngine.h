#pragma once
#include <vector>
#include "Particle.h"
#include <iostream>
#include <Windows.h>
#include <SDL_image.h>

class Particle; //Forward declaration for Particle so that Particle can have reference to the particle engine.

class ParticleEngine {

public:
	ParticleEngine(SDL_DisplayMode* newDisplayMode);

	void update(std::chrono::duration<double> delta, SDL_DisplayMode* displayMode);
	
	void render(SDL_Renderer* renderer);

	void spawnParticle(Particle* particle);

	void init(SDL_Renderer* renderer);

	unsigned int getDeleteCount();

	bool needsRendering();

	unsigned int getPointParticleCount() { return particles.size(); }

	void incrementPointDeleteCount() { pointDeleteCount++; }

	POINT getMousePosition() { return mousePos; }
	SDL_DisplayMode* getDisplayMode() { return displayMode; }

private:
	std::vector<Particle*> particles;

	POINT mousePos;
	SDL_DisplayMode* displayMode;

	unsigned int pointDeleteCount;

	SDL_Texture* particleTexture;

};

