#pragma once
#include <SDL.h>
#include <SDL_syswm.h>
#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "ParticleEngine.h"
#include "MouseEngine.h"
#include <SFML/Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iterator>
#include "AddPointParticle.h"
#include "LosePointParticle.h"


#define PI 3.14159265


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
	bool needsGraphicsFlush; //This is used to make sure screen is flushed after turning off rendering due to being idle. If this wasn't here, remnants would be left on the screen.
	double fps;
	ParticleEngine particleEngine;
	MouseEngine mouseEngine;

	unsigned int truePoints;

	std::string discordID;

	//Networking Stuff.
	sf::TcpSocket socket;
	void updateNetworking();

	bool shouldCatchup; //This is used so that we can catchup with server points without the entire flood of points.

	void appLogic();

	void setWindowVeiled(bool veil);


	//Overlay Stuff.
	bool isOverlayShown;
	bool isInTransitionAnimation;
	int transitionHeight;
	SDL_Texture* screenCaptureTexture;
	std::chrono::system_clock::time_point overlayCooldown;

	void toggleOverlay();
	void CaptureScreen();


	//Timing Stuff.
	std::chrono::system_clock::time_point fpsControlClock;
	std::chrono::system_clock::time_point fpsClock;
	std::chrono::system_clock::time_point networkClock;
	std::chrono::duration<double> lastTimeDifference;
};

