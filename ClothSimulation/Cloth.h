#pragma once
#include "Constraint.h"

class Cloth
{
private:

	int num_particles_width; // number of particles in "width" direction
	int num_particles_height; // number of particles in "height" direction
							  // total number of particles is num_particles_width*num_particles_height
	int m_num_hooks;
	float m_Width;

	std::vector<Particle> particles; // all particles that are part of this cloth
	std::vector<Constraint> constraints; // alle constraints between particles as part of this cloth

	//moved the getParticle function to public, so that Ben could usse it in Source.cpp
	void makeConstraint(Particle *p1, Particle *p2) { constraints.push_back(Constraint(p1, p2)); }

	Utils Util;

	glm::vec3 calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3);


	void addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3 direction);

	void drawTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3 color);

public:
	Particle* getParticle(int x, int y) { return &particles[y*num_particles_width + x]; }
	int getWidth() { return num_particles_width; }

	/* This is a important constructor for the entire system of particles and constraints*/
	Cloth(float width, float height, int num_particles_width, int num_particles_height, int num_hooks) : num_particles_width(num_particles_width), num_particles_height(num_particles_height)
	{
		m_num_hooks = num_hooks;
		m_Width = width;
		particles.resize(num_particles_width*num_particles_height); //I am essentially using this vector as an array with room for num_particles_width*num_particles_height particles

		// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
		for(int x=0; x<num_particles_width; x++)
		{
			for(int y=0; y<num_particles_height; y++)
			{
				glm::vec3 pos = glm::vec3(width * (x/(float)num_particles_width),
								-height * (y/(float)num_particles_height),
								0);
				particles[y*num_particles_width+x]= Particle(pos); // insert particle in column x at y'th row
			}
		}

		// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
		for(int x=0; x<num_particles_width; x++)
		{
			for(int y=0; y<num_particles_height; y++)
			{
				if (x<num_particles_width-1) makeConstraint(getParticle(x,y),getParticle(x+1,y));
				if (y<num_particles_height-1) makeConstraint(getParticle(x,y),getParticle(x,y+1));
				if (x<num_particles_width-1 && y<num_particles_height-1) makeConstraint(getParticle(x,y),getParticle(x+1,y+1));
				if (x<num_particles_width-1 && y<num_particles_height-1) makeConstraint(getParticle(x+1,y),getParticle(x,y+1));
			}
		}


		// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
		for(int x=0; x<num_particles_width; x++)
		{
			for(int y=0; y<num_particles_height; y++)
			{
				if (x<num_particles_width-2) makeConstraint(getParticle(x,y),getParticle(x+2,y));
				if (y<num_particles_height-2) makeConstraint(getParticle(x,y),getParticle(x,y+2));
				if (x<num_particles_width-2 && y<num_particles_height-2) makeConstraint(getParticle(x,y),getParticle(x+2,y+2));
				if (x<num_particles_width-2 && y<num_particles_height-2) makeConstraint(getParticle(x+2,y),getParticle(x,y+2));			}
		}

		//getParticle(0 + i, 0)->offsetPos(glm::vec3(0.25, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural
		//Top Left
		getParticle(0, 0)->makeUnmovable();
		getParticle(1, 0)->makeUnmovable();
		getParticle(0, 1)->makeUnmovable();

		//getParticle(num_particles_width - 1 - i, 0)->offsetPos(glm::vec3(-0.25, 0.0, 0.0));
		//Top Right
		getParticle(num_particles_width - 1, 0)->makeUnmovable();
		getParticle(num_particles_width - 1 - 1, 0)->makeUnmovable();
		getParticle(num_particles_width - 1, 1)->makeUnmovable();


		//Calculate and create extra hooks
		if (num_hooks > 2) {
			float LengthBetweenHooks = (float)num_particles_width / (float)(num_hooks - 1);

			//for each hook that isnt the courners, itterate through them making them un movable;
			for (int i = 0; i < num_hooks - 2; i++) {
				//Particle at the top of the hook (-1 as the cloth is 0 based)
				int TopParticle = glm::round((i + 1) * LengthBetweenHooks) - 1;
				getParticle(TopParticle, 0)->makeUnmovable();
				getParticle(TopParticle + 1, 1)->makeUnmovable();
				getParticle(TopParticle - 1, 1)->makeUnmovable();
			}
		}
	}

	Cloth();
	~Cloth();

	void drawShaded();

	/* this is an important methods where the time is progressed one time step for the entire cloth.
	This includes calling satisfyConstraint() for every constraint, and calling timeStep() for all particles
	*/
	void timeStep(float _Damping, GLfloat _DeltaTime, int _ConstraintIterations);

	/* used to add gravity (or any other arbitrary vector) to all particles*/
	void addForce(const glm::vec3 direction);

	/* used to add wind forces to all particles, is added for each triangle since the final force is proportional to the triangle area as seen from the wind direction*/
	void windForce(const glm::vec3 direction);

	void ballCollision(const glm::vec3 center, const float radius);

	void groundCollision(float _GroundHeight);

	void dropIt();

	void IncrementHookWidth(GLfloat _DeltaTime, float _Speed);
	void DecrementHookWidth(GLfloat _DeltaTime, float _Speed);

	void doFrame();
};