#pragma once

#include "Particle.h"
#include "Utils.h"

class Constraint
{
private:
	float rest_distance; // the length between particle p1 and p2 in rest configuration
	Utils Util;
public:
	Particle *p1, *p2; // the two particles that are connected through this constraint

	Constraint(Particle *p1, Particle *p2) : p1(p1), p2(p2)
	{
		glm::vec3 vec = p1->getPos() - p2->getPos();
		rest_distance = glm::length(vec);
	}

	Constraint();
	~Constraint();

	/* This is one of the important methods, where a single constraint between two particles p1 and p2 is solved
	the method is called by Cloth.time_step() many times per frame*/
	void satisfyConstraint();

};
