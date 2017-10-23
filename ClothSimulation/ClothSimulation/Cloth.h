#pragma once
#include "Constraint.h"

class Cloth
{
private:

	int num_particles_width; // number of particles in "width" direction
	int num_particles_height; // number of particles in "height" direction
							  // total number of particles is num_particles_width*num_particles_height

	std::vector<Particle> particles; // all particles that are part of this cloth
	std::vector<Constraint> constraints; // alle constraints between particles as part of this cloth

	Particle* getParticle(int x, int y) { return &particles[y*num_particles_width + x]; }
	void makeConstraint(Particle *p1, Particle *p2) { constraints.push_back(Constraint(p1, p2)); }

	Utils Util;
	/* A private method used by drawShaded() and addWindForcesForTriangle() to retrieve the
	normal vector of the triangle defined by the position of the particles p1, p2, and p3.
	The magnitude of the normal vector is equal to the area of the parallelogram defined by p1, p2 and p3
	*/
	glm::vec3 calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3);

	/* A private method used by windForce() to calcualte the wind force for a single triangle
	defined by p1,p2,p3*/
	void addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3 direction);

	/* A private method used by drawShaded(), that draws a single triangle p1,p2,p3 with a color*/
	void drawTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3 color);

public:

	/* This is a important constructor for the entire system of particles and constraints*/
	Cloth(float width, float height, int num_particles_width, int num_particles_height) : num_particles_width(num_particles_width), num_particles_height(num_particles_height)
	{
		particles.resize(num_particles_width*num_particles_height); //I am essentially using this vector as an array with room for num_particles_width*num_particles_height particles

																	// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
		for (int x = 0; x<num_particles_width; x++)
		{
			for (int y = 0; y<num_particles_height; y++)
			{
				glm::vec3 pos = glm::vec3(width * (x / (float)num_particles_width),
					-height * (y / (float)num_particles_height),
					0);
				particles[y*num_particles_width + x] = Particle(pos); // insert particle in column x at y'th row
			}
		}

		// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
		for (int x = 0; x<num_particles_width; x++)
		{
			for (int y = 0; y<num_particles_height; y++)
			{
				if (x<num_particles_width - 1) makeConstraint(getParticle(x, y), getParticle(x + 1, y));
				if (y<num_particles_height - 1) makeConstraint(getParticle(x, y), getParticle(x, y + 1));
				if (x<num_particles_width - 1 && y<num_particles_height - 1) makeConstraint(getParticle(x, y), getParticle(x + 1, y + 1));
				if (x<num_particles_width - 1 && y<num_particles_height - 1) makeConstraint(getParticle(x + 1, y), getParticle(x, y + 1));
			}
		}


		// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
		for (int x = 0; x<num_particles_width; x++)
		{
			for (int y = 0; y<num_particles_height; y++)
			{
				if (x<num_particles_width - 2) makeConstraint(getParticle(x, y), getParticle(x + 2, y));
				if (y<num_particles_height - 2) makeConstraint(getParticle(x, y), getParticle(x, y + 2));
				if (x<num_particles_width - 2 && y<num_particles_height - 2) makeConstraint(getParticle(x, y), getParticle(x + 2, y + 2));
				if (x<num_particles_width - 2 && y<num_particles_height - 2) makeConstraint(getParticle(x + 2, y), getParticle(x, y + 2));
			}
		}


		// making the upper left most three and right most three particles unmovable
		for (int i = 0; i<3; i++)
		{
			getParticle(0 + i, 0)->offsetPos(glm::vec3(0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			getParticle(0 + i, 0)->makeUnmovable();

			getParticle(0 + i, 0)->offsetPos(glm::vec3(-0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			getParticle(num_particles_width - 1 - i, 0)->makeUnmovable();
		}
	}

	Cloth();
	~Cloth();

	/* drawing the cloth as a smooth shaded (and colored according to column) OpenGL triangular mesh
	Called from the display() method
	The cloth is seen as consisting of triangles for four particles in the grid as follows:

	(x,y)   *--* (x+1,y)
	| /|
	|/ |
	(x,y+1) *--* (x+1,y+1)

	*/
	void drawShaded();

	/* this is an important methods where the time is progressed one time step for the entire cloth.
	This includes calling satisfyConstraint() for every constraint, and calling timeStep() for all particles
	*/
	void timeStep(float _Damping, GLfloat _DeltaTime, int _ConstraintIterations);

	/* used to add gravity (or any other arbitrary vector) to all particles*/
	void addForce(const glm::vec3 direction);

	/* used to add wind forces to all particles, is added for each triangle since the final force is proportional to the triangle area as seen from the wind direction*/
	void windForce(const glm::vec3 direction);

	/* used to detect and resolve the collision of the cloth with the ball.
	This is based on a very simples scheme where the position of each particle is simply compared to the sphere and corrected.
	This also means that the sphere can "slip through" if the ball is small enough compared to the distance in the grid bewteen particles
	*/
	void ballCollision(const glm::vec3 center, const float radius);

	void doFrame();
};


