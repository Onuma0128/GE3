#pragma once

#include <list>
#include <string>

#include "Collider.h"

class CollisionManager
{
public:

	void Reset();

	void AddCollider(Collider* collider);

	void CheckAllCollisions();

private:

	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

private:

	std::list<Collider*> colliders_;

};