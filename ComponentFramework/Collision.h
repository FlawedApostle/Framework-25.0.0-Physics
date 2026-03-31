#pragma once
#include "Body.h"
// This functions are to calculate the DISTANCE between two objects Body* in this case
namespace Collision {

	// Means we are promising to look at but not change the Body objects. We are just detecting collisions after all
	bool SphereSphereCollisionDetected(const Body* body1, const Body* body2);

	// If a collision has occurred, we need to immediately change the velocity 
	// of both spheres. We will definitely be changing the Body object’s so there are  no “const” keywords here
	void SphereSphereCollisionResponse(Body* body1, Body* body2);



	// DEPRECATED FUNCTION
	bool SphereSphereCollisionDetected_test(Vec3 body1, Vec3 body2);
	bool SphereSphereCollisionDetected_test(Body* body1 , Body* body2);
}