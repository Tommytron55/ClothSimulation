#include "Cloth.h"



Cloth::Cloth()
{
}


Cloth::~Cloth()
{
}


glm::vec3 Cloth::calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3)
{
	glm::vec3 pos1 = p1->getPos();
	glm::vec3 pos2 = p2->getPos();
	glm::vec3 pos3 = p3->getPos();

	glm::vec3 v1 = pos2 - pos1;
	glm::vec3 v2 = pos3 - pos1;

	return glm::cross(v1, v2);
}

/* A private method used by windForce() to calcualte the wind force for a single triangle
defined by p1,p2,p3*/
void Cloth::addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3 direction)
{
	glm::vec3 normal = calcTriangleNormal(p1, p2, p3);
	glm::vec3 d = glm::normalize(normal);
	glm::vec3 force = normal* glm::dot(d, direction);
	p1->addForce(force);
	p2->addForce(force);
	p3->addForce(force);
}

/* A private method used by drawShaded(), that draws a single triangle p1,p2,p3 with a color*/
void Cloth::drawTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3 color)
{
	glColor3fv((GLfloat*)&color);

	glNormal3fv((GLfloat *) &(glm::normalize(p1->getNormal())));
	glVertex3fv((GLfloat *) &(p1->getPos()));

	glNormal3fv((GLfloat *) &(glm::normalize(p2->getNormal())));
	glVertex3fv((GLfloat *) &(p2->getPos()));

	glNormal3fv((GLfloat *) &(glm::normalize(p3->getNormal())));
	glVertex3fv((GLfloat *) &(p3->getPos()));
}


////PUBLICS//////////////////////////////////////////////////////////////////////////////////////////////



void Cloth::drawShaded()
{
	// reset normals (which where written to last frame)
	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		(*particle).resetNormal();
	}

	//create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
	for (int x = 0; x<num_particles_width - 1; x++)
	{
		for (int y = 0; y<num_particles_height - 1; y++)
		{
			glm::vec3 normal = calcTriangleNormal(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1));
			getParticle(x + 1, y)->addToNormal(normal);
			getParticle(x, y)->addToNormal(normal);
			getParticle(x, y + 1)->addToNormal(normal);

			normal = calcTriangleNormal(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1));
			getParticle(x + 1, y + 1)->addToNormal(normal);
			getParticle(x + 1, y)->addToNormal(normal);
			getParticle(x, y + 1)->addToNormal(normal);
		}
	}

	glBegin(GL_TRIANGLES);
	for (int x = 0; x<num_particles_width - 1; x++)
	{
		for (int y = 0; y<num_particles_height - 1; y++)
		{
			glm::vec3 color(0, 0, 0);
			/*if (x % 7 == 0) // red and white color is interleaved according to which column number
			//	color = glm::vec3(0.6f, 0.2f, 0.6f);
			//else if (x % 5 == 0) // red and white color is interleaved according to which column number
			//	color = glm::vec3(0.6f, 0.6f, 0.2f);
			else*/ if (x % 3 == 0) // red and white color is interleaved according to which column number
				color = glm::vec3(0.2f, 0.6f, 0.2f);
			else if (x % 2 == 0) // red and white color is interleaved according to which column number
				color = glm::vec3(0.2f, 0.4f, 0.4f);
			else
				color = glm::vec3(0.2f, 0.2f, 0.6f);

			drawTriangle(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1), color);
			drawTriangle(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), color);
		}
	}
	glEnd();
}

/* this is an important methods where the time is progressed one time step for the entire cloth.
This includes calling satisfyConstraint() for every constraint, and calling timeStep() for all particles
*/
void Cloth::timeStep(float _Damping, GLfloat _DeltaTime, int _ConstraintIterations)
{
	std::vector<Constraint>::iterator constraint;
	for (int i = 0; i<_ConstraintIterations; i++) // iterate over all constraints several times
	{
		for (constraint = constraints.begin(); constraint != constraints.end(); constraint++)
		{
			(*constraint).satisfyConstraint(); // satisfy constraint.
		}
	}

	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		(*particle).timeStep(_Damping, _DeltaTime); // calculate the position of each particle at the next time step.
	}
}

/* used to add gravity (or any other arbitrary vector) to all particles*/
void Cloth::addForce(const glm::vec3 direction)
{
	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		(particle)->addForce(direction); // add the forces to each particle
	}
}

/* used to add wind forces to all particles, is added for each triangle since the final force is proportional to the triangle area as seen from the wind direction*/
void Cloth::windForce(const glm::vec3 direction)
{
	for (int x = 0; x<num_particles_width - 1; x++)
	{
		for (int y = 0; y<num_particles_height - 1; y++)
		{
			addWindForcesForTriangle(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1), direction);
			addWindForcesForTriangle(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), direction);
		}
	}
}

/* used to detect and resolve the collision of the cloth with the ball.
This is based on a very simples scheme where the position of each particle is simply compared to the sphere and corrected.
This also means that the sphere can "slip through" if the ball is small enough compared to the distance in the grid bewteen particles
*/
void Cloth::ballCollision(const glm::vec3 center, const float radius)
{
	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		glm::vec3 v = (*particle).getPos() - center;
		float length = glm::length(v);
		if (length < radius) // if the particle is inside the ball
		{
			(*particle).offsetPos(Util.ScaleVec3(glm::normalize(v), (radius - length))); // project the particle to the surface of the ball
		}
	}
}

static float randFloat(float min, float max) {
	float r = (float)rand() / (double)RAND_MAX;
	return min + r * (max - min);
}

void Cloth::groundCollision(float _GroundHeight) {
	int iCounter = 1;
	int TotalCount = num_particles_width * (num_particles_height - 1);
	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		if (iCounter > TotalCount)
		{
			if (particle->getPos().y < _GroundHeight) {
				if (!(*particle).getCollided())
				{
					(*particle).offsetPos(glm::vec3(0.0f, _GroundHeight - particle->getPos().y, -0.5f));
					(*particle).setCollided();
				}
				else
				{
					(*particle).offsetPos(glm::vec3(0.0f, _GroundHeight - particle->getPos().y, -0.0f));
				}
			}
		}
		else
		{
			if (particle->getPos().y < _GroundHeight) {
				(*particle).offsetPos(glm::vec3(0.0f, _GroundHeight - particle->getPos().y, -0.0f));
			}
		}
		iCounter++;
	}
}

void Cloth::dropIt() {
	int iCounter = 1;
	int TotalCount = num_particles_width * 2;
	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		particle->makeMovable();
		if (iCounter >= TotalCount)
			break;

		iCounter++;
	}
}


void Cloth::IncrementHookWidth(GLfloat _DeltaTime, float _Speed) {

	//Top Left
	float Distance = (m_Width / 2.0f) - getParticle(0, 0)->getPos().x;
	float Scale = Distance / (m_Width / 2.0f);
	getParticle(0, 0)->offsetUnmovable(glm::vec3(Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

	Distance = (m_Width / 2.0f) - getParticle(1, 0)->getPos().x;
	Scale = Distance / (m_Width / 2.0f);
	getParticle(1, 0)->offsetUnmovable(glm::vec3(Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

	Distance = (m_Width / 2.0f) - getParticle(0, 1)->getPos().x;
	Scale = Distance / (m_Width / 2.0f);
	getParticle(0, 1)->offsetUnmovable(glm::vec3(Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

	//Top Right
	Distance = (m_Width / 2.0f) - getParticle(num_particles_width - 1, 0)->getPos().x;
	Scale = Distance / (m_Width / 2.0f);
	getParticle(num_particles_width - 1, 0)->offsetUnmovable(glm::vec3(Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

	Distance = (m_Width / 2.0f) - getParticle(num_particles_width - 1 -1, 0)->getPos().x;
	Scale = Distance / (m_Width / 2.0f);
	getParticle(num_particles_width - 1 - 1, 0)->offsetUnmovable(glm::vec3(Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

	Distance = (m_Width / 2.0f) - getParticle(num_particles_width - 1, 1)->getPos().x;
	Scale = Distance / (m_Width / 2.0f);
	getParticle(num_particles_width - 1, 1)->offsetUnmovable(glm::vec3(Scale * _DeltaTime * _Speed, 0.0f, 0.0f));


	if (m_num_hooks >= 2) {
		float LengthBetweenHooks = (float)num_particles_width / (float)(m_num_hooks - 1);

		//for each hook that isnt the courners, itterate through them making them un movable;
		for (int i = 0; i < m_num_hooks - 2; i++) {
			//Particle at the top of the hook (-1 as the cloth is 0 based)
			int TopParticle = glm::round((i + 1) * LengthBetweenHooks) - 1;
			Distance = (m_Width / 2.0f) - getParticle(TopParticle, 0)->getPos().x;
			Scale = Distance / (m_Width / 2.0f);
			getParticle(TopParticle, 0)->offsetUnmovable(glm::vec3(Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

			Distance = (m_Width / 2.0f) - getParticle(TopParticle + 1, 1)->getPos().x;
			Scale = Distance / (m_Width / 2.0f);
			getParticle(TopParticle + 1, 1)->offsetUnmovable(glm::vec3(Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

			Distance = (m_Width / 2.0f) - getParticle(TopParticle - 1, 1)->getPos().x;
			Scale = Distance / (m_Width / 2.0f);
			getParticle(TopParticle - 1, 1)->offsetUnmovable(glm::vec3(Scale * _DeltaTime * _Speed, 0.0f, 0.0f));
		}
	}
}


void Cloth::DecrementHookWidth(GLfloat _DeltaTime, float _Speed) {

	//Top Left
	float Distance = (m_Width / 2.0f) - getParticle(0, 0)->getPos().x;
	float Scale = Distance / (m_Width / 2.0f);
	getParticle(0, 0)->offsetUnmovable(glm::vec3(-Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

	Distance = (m_Width / 2.0f) - getParticle(1, 0)->getPos().x;
	Scale = Distance / (m_Width / 2.0f);
	getParticle(1, 0)->offsetUnmovable(glm::vec3(-Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

	Distance = (m_Width / 2.0f) - getParticle(0, 1)->getPos().x;
	Scale = Distance / (m_Width / 2.0f);
	getParticle(0, 1)->offsetUnmovable(glm::vec3(-Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

	//Top Right
	Distance = (m_Width / 2.0f) - getParticle(num_particles_width - 1, 0)->getPos().x;
	Scale = Distance / (m_Width / 2.0f);
	getParticle(num_particles_width - 1, 0)->offsetUnmovable(glm::vec3(-Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

	Distance = (m_Width / 2.0f) - getParticle(num_particles_width - 1 - 1, 0)->getPos().x;
	Scale = Distance / (m_Width / 2.0f);
	getParticle(num_particles_width - 1 - 1, 0)->offsetUnmovable(glm::vec3(-Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

	Distance = (m_Width / 2.0f) - getParticle(num_particles_width - 1, 1)->getPos().x;
	Scale = Distance / (m_Width / 2.0f);
	getParticle(num_particles_width - 1, 1)->offsetUnmovable(glm::vec3(-Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

	if (m_num_hooks >= 2) {
		float LengthBetweenHooks = (float)num_particles_width / (float)(m_num_hooks - 1);

		//for each hook that isnt the courners, itterate through them making them un movable;
		for (int i = 0; i < m_num_hooks - 2; i++) {
			//Particle at the top of the hook (-1 as the cloth is 0 based)
			int TopParticle = glm::round((i + 1) * LengthBetweenHooks) - 1;
			float Distance = (m_Width / 2.0f) - getParticle(TopParticle, 0)->getPos().x;
			float Scale = Distance / (m_Width / 2.0f);
			getParticle(TopParticle, 0)->offsetUnmovable(glm::vec3(-Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

			Distance = (m_Width / 2.0f) - getParticle(TopParticle + 1, 1)->getPos().x;
			Scale = Distance / (m_Width / 2.0f);
			getParticle(TopParticle + 1, 1)->offsetUnmovable(glm::vec3(-Scale * _DeltaTime * _Speed, 0.0f, 0.0f));

			Distance = (m_Width / 2.0f) - getParticle(TopParticle - 1, 1)->getPos().x;
			Scale = Distance / (m_Width / 2.0f);
			getParticle(TopParticle - 1, 1)->offsetUnmovable(glm::vec3(-Scale * _DeltaTime * _Speed, 0.0f, 0.0f));
		}
	}
}

void Cloth::doFrame()
{

}