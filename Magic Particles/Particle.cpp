#include "Particle.h"

Particle::Particle(MathVector point) {
	position = MathVector(point.x, point.y);
	shouldDie = false;
	shouldGivePointsOnDeath = false;
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

void Particle::update(std::chrono::duration<double> delta, ParticleEngine* pEngine) {
	velocity = velocity + (acceleration * delta.count());
	if (velocityCap != 0 && velocity.getMagnitude() > velocityCap) { //Reduce velocity to cap if needed.
		velocity = (velocity.getUnitVector() * velocityCap);
	}
	position = position + (velocity * delta.count());

	

	//std::cout << "Position: " << position.x << ", " << position.y << std::endl;
	//std::cout << "Velocity: " << velocity.x << ", " << velocity.y << std::endl;
	//std::cout << "Acceleration: " << acceleration.x << ", " << acceleration.y << std::endl;
	
}
