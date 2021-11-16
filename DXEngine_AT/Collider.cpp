#include "Collider.h"

bool Collider::RectCollision(RectColliderObject target)
{
	RectColliderObject a = this->colliderObject;
	RectColliderObject b = target;

	return a.max.x > b.min.x && a.min.x < b.max.x && a.max.z > b.min.z && a.min.z < b.max.z;
}

void Collider::CalculateAABB(Vector3 p)
{
	colliderObject.min = p - size;
	colliderObject.max = p + size;
}
