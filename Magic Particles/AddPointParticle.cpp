#include "AddPointParticle.h"

AddPointParticle::AddPointParticle(MathVector point, POINT* newMousePosition, SDL_DisplayMode* newDisplayMode): Particle(point, newMousePosition, newDisplayMode) {
	velocityCap = 400; //Default for add point particle.
	particleType = ParticleType::AddPointParticleType;

}

void AddPointParticle::update(std::chrono::duration<double> delta) {

	Particle::update(delta); // Call to super function for basics.


	//Set Acceleration.

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
