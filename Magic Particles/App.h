#pragma once
#include <SDL.h>
#include <SDL_syswm.h>
#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "ParticleEngine.h"


class App{

public:
	App();


	void close() { shouldRun = false; }
	void setFPS(double newFPS) { fps = newFPS; }

	void mainLoop();


protected:

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_SysWMinfo info;
	SDL_DisplayMode DM;

	//Loop Stuff.
	void render();
	void update();
	void cleanup();

private:
	bool shouldRun;
	double fps;
	ParticleEngine particleEngine;


	//Timing Stuff.
	std::chrono::system_clock::time_point fpsControlClock;
	std::chrono::system_clock::time_point fpsClock;
	std::chrono::duration<double> lastTimeDifference;
};

