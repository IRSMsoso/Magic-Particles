#include "ParticleEngine.h"


bool isFullscreen(HWND); //Forward declaration for class-independent function below.
bool cursorShown();


ParticleEngine::ParticleEngine(SDL_DisplayMode* newDisplayMode) {
	addPointParticleTexture = nullptr;
	losePointParticleTexture = nullptr;
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

			if (particles.at(i)->getParticleType() == ParticleType::AddPointParticleType) {
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

bool ParticleEngine::needsRendering()
{
	QUERY_USER_NOTIFICATION_STATE pquns;
	SHQueryUserNotificationState(&pquns);

	return ((particles.size() != 0) && !isFullscreen(GetForegroundWindow()) && cursorShown());
	
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