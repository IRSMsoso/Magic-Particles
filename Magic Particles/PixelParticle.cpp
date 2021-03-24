#include "PixelParticle.h"

PixelParticle::PixelParticle(MathVector point, POINT* mousePosition, SDL_DisplayMode* newDisplaymode, Uint32* newColor, int newSize): Particle(point, mousePosition, newDisplaymode) {
	velocityCap = 400;
	particleType = ParticleType::PixelParticleType;
	color = newColor; //Might be expensive.
	size = newSize;
}

PixelParticle::~PixelParticle() {
	delete color;
}

void PixelParticle::update(std::chrono::duration<double> delta) {
	Particle::update(delta); //Can discard and do ourselves if we need more speed. Or just rework basics :shrug:


	//MathVector diffVector = MathVector(mousePosition->x + 4.0, mousePosition->y + 9.0) - position; //Correction to position inside the mouse vector.

	//acceleration = diffVector / 20.0;

	MathVector diffVector = MathVector(mousePosition->x + 4.0, mousePosition->y + 9.0) - position; //Correction to position inside the mouse vector.
	MathVector accVector = (diffVector.getUnitVector() / pow(diffVector.getMagnitude(), 2)) * 100000000.0;

	acceleration = accVector;


	MathVector particlePosition = position;
	MathVector particleVelocity = velocity;

	if (particlePosition.x < 0)
		velocity = MathVector(abs(particleVelocity.x), particleVelocity.y);
	else if (particlePosition.x > displayMode->w)
		velocity = MathVector(abs(particleVelocity.x) * -1, particleVelocity.y);
	if (particlePosition.y < 0)
		velocity = MathVector(particleVelocity.x, abs(particleVelocity.y));
	else if (particlePosition.y > displayMode->h)
		velocity = MathVector(particleVelocity.x, abs(particleVelocity.y) * -1);

	//Mark for delete of this particle is too close.
	if (diffVector.getMagnitude() < 3) {
		shouldDie = true;
	}
}
