#include "Collision.h"

#include <cmath>
#include <cmath>

bool Collision::SphereSphereCollisionDetected(const Body* body1, const Body* body2)
{
	double _pointPosition = sqrt(pow((body1->pos.x + body2->pos.x), 2) + pow((body1->pos.y + body2->pos.y) , 2));
	printf("Point Position: %f\n" ,_pointPosition);

	return _pointPosition;
}

bool Collision::SphereSphereCollisionDetected_test(const Vec2 body1, const Vec2 body2)
{
	double _pointPosition = sqrt(pow((body1.x + body2.x), 2) + pow((body1.y + body2.y), 2));
	//printf("Point Position: %f\n", _pointPosition);

	return _pointPosition;
}
