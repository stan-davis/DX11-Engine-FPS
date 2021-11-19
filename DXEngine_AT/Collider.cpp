#include "Collider.h"
#include <cmath>
#include <algorithm>

bool Collider::CircleRectCollision(Vector3 target, RectColliderObject object, Vector3& resolutionOut)
{
	//Algorithm from Javidx9 (Circle Vs Rectangle Collisions)
	Vector3 minPoint = object.min;
	Vector3 maxPoint = object.max;

	//Find the nearest point on the collider
	Vector3 nearest = { 0,0,0 };
	nearest.x = std::max(minPoint.x, std::min(target.x, maxPoint.x));
	nearest.z = std::max(minPoint.z, std::min(target.z, maxPoint.z));

	//Cast a ray out to that point and check the overlapping amount
	Vector3 ray = nearest - target;
	float overlap = radius - ray.Magnitude();

	if (std::isnan(overlap)) overlap = 0;

	if (overlap > 0)
	{
		ray.Normalize();
		resolutionOut = target - ray * overlap;
		return true;
	}
	else
	{
		resolutionOut = target;
		return false;
	}
}

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
