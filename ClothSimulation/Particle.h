#pragma once
#include "Utils.h"

class Particle
{
private:
	bool movable; // can the particle move or not ? used to pin parts of the cloth

	float mass; // the mass of the particle (is always 1 in this example)
	glm::vec3 pos; // the current position of the particle in 3D space
	glm::vec3 old_pos; // the position of the particle in the previous time step, used as part of the verlet numerical integration scheme
	glm::vec3 acceleration; // a vector representing the current acceleration of the particle
	glm::vec3 accumulated_normal; // an accumulated normal (i.e. non normalized), used for OpenGL soft shading

public:
	Particle(glm::vec3 pos) : pos(pos), old_pos(pos), acceleration(glm::vec3(0, 0, 0)), mass(1), movable(true), accumulated_normal(glm::vec3(0, 0, 0)) {}
	Particle();
	~Particle();

	void addForce(glm::vec3 f);

	/* The method is called by Cloth.time_step()
	Given the equation "force = mass * acceleration" the next position is found through verlet integration*/
	void timeStep(float _Damping, GLfloat _DeltaTime);

	glm::vec3& getPos();

	void resetAcceleration();

	void offsetPos(const glm::vec3 v);

	void makeUnmovable();

	void makeMovable();

	bool getMoveable();

	void addToNormal(glm::vec3 normal);

	glm::vec3& getNormal();

	void resetNormal();

};