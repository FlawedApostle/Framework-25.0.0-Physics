#include "Collision.h"

#include <cmath>
#include <cmath>

bool Collision::SphereSphereCollisionDetected(const Body* body1, const Body* body2)
{
	//double _pointPosition = sqrt(pow((body1->pos.x - body2->pos.x), 2) + pow((body1->pos.y - body2->pos.y) , 2));
	//printf("Point Position: %f\n" ,_pointPosition);
	//return _pointPosition;

	Vec3 d = body1->pos - body2->pos;
	float distanceSquared = VMath::dot(d, d);

	float radiusSum = body1->radius + body2->radius;
	float radiusSumSquared = radiusSum * radiusSum;

	bool collided = distanceSquared <= radiusSumSquared;

	if (collided)
		printf("Collision!  distSq=%.3f  radSq=%.3f\n", distanceSquared, radiusSumSquared);
	else
		printf("No Collision. distSq=%.3f  radSq=%.3f\n", distanceSquared, radiusSumSquared);

	return collided;
}



// DEPRECATED
bool Collision::SphereSphereCollisionDetected_test(Vec3 body1, Vec3 body2)
{
	double _pointPosition = sqrt(pow((body1.x - body2.x), 2) + pow((body1.y - body2.y), 2) + pow((body1.z - body2.z), 2));
	return _pointPosition;
}


bool Collision::SphereSphereCollisionDetected_test(Body* body1, Body* body2)
{
	//double newPoint = sqrt(pow((body1->pos.x - body2->pos.x), 2) - pow((body1->pos.y - body2->pos.y), 2));
	float dx = body1->pos.x - body2->pos.x;
	float dy = body1->pos.y - body2->pos.y;
	
	// Squared distance formula - there is NO sqrt because its GPU heavy
	// The logic follows same suit - rather than getting the square root value its taxed as a squared distance instead 
	// if a < b THEN pow(a,2) < pow(b,2)
	// if a = b THEN pow(a,2) = pow(b,2)
	float distanceSquared = (dx * dx) + (dy * dy);
	float radiusSum = body1->radius + body2->radius;
	float radiusSumSquared = radiusSum * radiusSum;

	if (distanceSquared <= radiusSum * radiusSumSquared) {
		printf("Collision!\n");
		return true;
	}

	printf("No-Collision!\n");
	return false;
}
