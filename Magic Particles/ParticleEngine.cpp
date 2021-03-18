#include "ParticleEngine.h"


bool isFullscreen(HWND); //Forward declaration for class-independent function below.
bool cursorShown();


ParticleEngine::ParticleEngine(SDL_DisplayMode* newDisplayMode) {
	particleTexture = NULL;
	pointDeleteCount = 0;
	displayMode = newDisplayMode;
}

void ParticleEngine::update(std::chrono::duration<double> delta, SDL_DisplayMode* displayMode) {
	GetCursorPos(&mousePos);

	for (int i = 0; i < particles.size(); i++) {
		//Call update here.

		particles.at(i)->update(delta, this);


		//Delete particles that should be deleted.
		if (particles.at(i)->getShouldDelete()) {

			if (particles.at(i)->getShouldGivePointsOnDeath()) {
				incrementPointDeleteCount();
			}
			

			delete particles.at(i);
			particles.erase(particles.begin() + i);
			i--;
		}
	}


	
}

void ParticleEngine::render(SDL_Renderer* renderer) {
	for (int i = 0; i < particles.size(); i++) {
		SDL_Point point = particles.at(i)->getNearestIntPoint();
		//SDL_RenderDrawPoint(renderer, point.x, point.y);
		SDL_Rect rect;
		rect.x = point.x;
		rect.y = point.y;
		rect.w = 6;
		rect.h = 6;
		SDL_RenderCopy(renderer, particleTexture, NULL, &rect);
	}
}

void ParticleEngine::spawnParticle(Particle* particle) {
	particles.push_back(particle);
}

void ParticleEngine::init(SDL_Renderer* renderer) {
	particleTexture = IMG_LoadTexture(renderer, "magicparticle1.png");
	if (particleTexture == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", "magicparticle1.png", IMG_GetError());
	}
}

unsigned int ParticleEngine::getDeleteCount()
{
	unsigned int returnVal = pointDeleteCount;
	pointDeleteCount = 0;
	return returnVal;
}

bool ParticleEngine::needsRendering()
{
	QUERY_USER_NOTIFICATION_STATE pquns;
	SHQueryUserNotificationState(&pquns);

	return ((particles.size() != 0) && !isFullscreen(GetForegroundWindow()) && cursorShown());
	
}


//This function is taken from https://stackoverflow.com/questions/7009080/detecting-full-screen-mode-in-windows
bool isFullscreen(HWND windowHandle)
{
	MONITORINFO monitorInfo = { 0 };
	monitorInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

	RECT windowRect;
	GetWindowRect(windowHandle, &windowRect);

	return windowRect.left == monitorInfo.rcMonitor.left
		&& windowRect.right == monitorInfo.rcMonitor.right
		&& windowRect.top == monitorInfo.rcMonitor.top
		&& windowRect.bottom == monitorInfo.rcMonitor.bottom;
}

bool cursorShown() {
	CURSORINFO ci = { sizeof(CURSORINFO) };

	if (GetCursorInfo(&ci)) {
		if (ci.flags == 1) {
			return true;
		}
	}
	return false;
}