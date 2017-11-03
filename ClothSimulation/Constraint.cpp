#include "Constraint.h"

Constraint::Constraint()
{
}


Constraint::~Constraint()
{
}



void Constraint::satisfyConstraint()
{
	glm::vec3 p1_to_p2 = p2->getPos() - p1->getPos(); // vector from p1 to p2
	float current_distance = glm::length(p1_to_p2); // current distance between p1 and p2
	glm::vec3 correctionVector = Util.ScaleVec3(p1_to_p2, (1 - rest_distance / current_distance)); // The offset vector that could moves p1 into a distance of rest_distance to p2
	glm::vec3 correctionVectorHalf = Util.ScaleVec3(correctionVector, 0.25f); // Lets make it half that length, so that we can move BOTH p1 and p2.
	p1->offsetPos(correctionVectorHalf); // correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint.
	p2->offsetPos(-correctionVectorHalf); // we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2.	
}