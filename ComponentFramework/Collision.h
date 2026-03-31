#pragma once
#include "Body.h"
namespace Collision {

	// Means we are promising to look at but not change the Body objects. We are just detecting collisions after all
	bool SphereSphereCollisionDetected(const Body* body1, const Body* body2);

	// If a collision has occurred, we need to immediately change the velocity 
	// of both spheres. We will definitely be changing the Body object’s so there are  no “const” keywords here
	//void SphereSphereCollisionResponse(PhysicsComponent* body1, PhysicsComponent* body2);



	// DEPRECATED FUNCTION
	bool SphereSphereCollisionDetected_test(const Vec2 body1, const Vec2 body2);
}