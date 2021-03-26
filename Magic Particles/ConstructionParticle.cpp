#include "ConstructionParticle.h"


ConstructionParticle::ConstructionParticle(POINT* newBasePosition, int newOffsetX, int newOffsetY, SDL_DisplayMode* displayMode, Uint32* newColor, int newSize): Particle(MathVector(displayMode->w / 2.0, displayMode->h / 2.0), nullptr, displayMode) {
	velocityCap = 200;
	particleType = ParticleType::ConstructionParticleType;
	color = newColor;
	size = newSize;
	basePosition = newBasePosition;
	offsetX = newOffsetX;
	offsetY = newOffsetY;
	isConstructed = false;
}

ConstructionParticle::~ConstructionParticle() {
	delete color;
}

void ConstructionParticle::update(std::chrono::duration<double> delta) {

	Particle::update(delta);

	if (isConstructed) {
		position.x = basePosition->x + offsetX;
		position.y = basePosition->y + offsetY;
		//std::cout << "Particles ready for construction\n";
	}
	else {
		MathVector diffVector = MathVector(basePosition->x + offsetX, basePosition->y + offsetY) - position; //Correction to destination position.
		MathVector accVector = (diffVector.getUnitVector() / pow(diffVector.getMagnitude(), 2)) * 100000000.0;

		acceleration = accVector;

		//std::cout << "Current Position: " << position.x << ", " << position.y << std::endl;
		//std::cout << "Base Position: " << basePosition->x << ", " << basePosition->y << std::endl;
		//std::cout << "Trying to get to: " << basePosition->x + offsetX << ", " << basePosition->y + offsetY << std::endl;


		//Mark as constructed if particle is close enough..
		if (diffVector.getMagnitude() < 3) {
			isConstructed = true;
			velocity.x = 0;
			velocity.y = 0;
			acceleration.x = 0;
			acceleration.y = 0;
		}
	}
}
