#pragma once
#include <vector>
#include "Particle.h"
#include <iostream>
#include <Windows.h>
#include <SDL_image.h>
#include "PixelParticle.h"



class ParticleEngine {

public:
	ParticleEngine(SDL_DisplayMode* newDisplayMode);

	void update(std::chrono::duration<double> delta, SDL_DisplayMode* displayMode);
	
	void render(SDL_Renderer* renderer);

	void spawnParticle(Particle* particle);

	void init(SDL_Renderer* renderer);

	unsigned int getDeleteCount();

	bool hasParticlesToRender();

	unsigned int getPointParticleCount();

	void clearParticlesOfType(ParticleType type);

	void incrementPointDeleteCount() { pointDeleteCount++; }

	POINT* getMousePosition() { return &mousePos; }
	SDL_DisplayMode* getDisplayMode() { return displayMode; }

private:
	std::vector<Particle*> particles;

	POINT mousePos;
	SDL_DisplayMode* displayMode;

	unsigned int pointDeleteCount;

	//Various Textures
	SDL_Texture* addPointParticleTexture;
	SDL_Texture* losePointParticleTexture;

};

