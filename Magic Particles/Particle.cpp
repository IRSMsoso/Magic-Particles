#include "Particle.h"

Particle::Particle(MathVector point, POINT* newMousePosition, SDL_DisplayMode* newDisplayMode) {
	position = MathVector(point.x, point.y);
	shouldDie = false;
	particleType = ParticleType::ParticleErrorType;
	birthTime = std::chrono::system_clock::now();
	lifeSpan = std::chrono::seconds(0);
	mousePosition = newMousePosition;
	displayMode = newDisplayMode;
}

Particle::~Particle()
{
}

void Particle::setPosition(MathVector newPosition) {
	position = newPosition;
}

void Particle::setVelocity(MathVector newVelocity) {
	velocity = newVelocity;
}

void Particle::setAcceleration(MathVector newAcceleration) {
	acceleration = newAcceleration;
}

SDL_Point Particle::getNearestIntPoint()
{
	SDL_Point point;
	point.x = round(position.x);
	point.y = round(position.y);
	return point;
}

void Particle::update(std::chrono::duration<double> delta) {
	if (!shouldDie) {
		velocity = velocity + (acceleration * delta.count());
		if (velocityCap != 0 && velocity.getMagnitude() > velocityCap) { //Reduce velocity to cap if needed.
			velocity = (velocity.getUnitVector() * velocityCap);
		}
		position = position + (velocity * delta.count());


		if (lifeSpan.count() > 0) {
			if (std::chrono::system_clock::now() > birthTime + lifeSpan) {
				shouldDie = true;
			}
		}

	}

	//std::cout << "Position: " << position.x << ", " << position.y << std::endl;
	//std::cout << "Velocity: " << velocity.x << ", " << velocity.y << std::endl;
	//std::cout << "Acceleration: " << acceleration.x << ", " << acceleration.y << std::endl;
	
}
