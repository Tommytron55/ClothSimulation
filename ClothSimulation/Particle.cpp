#include "Particle.h"

Particle::Particle() {};

Particle::~Particle() {};

void Particle::addForce(glm::vec3 f)
{
	acceleration.x += f.x / mass;
	acceleration.y += f.y / mass;
	acceleration.z += f.z / mass;
}

void Particle::timeStep(float _Damping, GLfloat _DeltaTime)
{
	if (movable)
	{
		glm::vec3 temp = pos;
		glm::vec3 temp2 = pos - old_pos;
		temp2.x *= (1.0 - _Damping);
		temp2.y *= (1.0 - _Damping);
		temp2.z *= (1.0 - _Damping);
		pos = pos + temp2 + acceleration*_DeltaTime;
		old_pos = temp;
		acceleration = glm::vec3(0, 0, 0); // acceleration is reset since it HAS been translated into a change in position (and implicitely into velocity)	
	}
}

glm::vec3& Particle::getPos() { 
	return pos; 
}

void Particle::resetAcceleration() { 
	acceleration = glm::vec3(0, 0, 0);
}

void Particle::offsetPos(const glm::vec3 v) {
	if (movable) 
		pos += v; 
}

void Particle::makeUnmovable() {
	movable = false;
}

void Particle::makeMovable() {
	movable = true;
}

bool Particle::getMoveable()
{
	return movable;
}

void Particle::addToNormal(glm::vec3 normal)
{
	accumulated_normal += glm::normalize(normal);;
}

glm::vec3& Particle::getNormal() {
	return accumulated_normal;
} // notice, the normal is not unit length

void Particle::resetNormal() { 
	accumulated_normal = glm::vec3(0, 0, 0);
}