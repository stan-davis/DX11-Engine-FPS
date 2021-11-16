#pragma once
#include "UtilityMath.h"
#include <DirectXMath.h>
#include "UtilityMath.h"
#include <vector>

namespace DX = DirectX;

class Collider
{
public:
	struct RectColliderObject
	{
		Vector3 min = {0,0,0};
		Vector3 max = {0,0,0};
	};

	Collider() = default;
	Collider(Vector3 _size) : size(_size) {}
	~Collider() = default;

	RectColliderObject GetColliderObject() { return colliderObject; }

	bool RectCollision(RectColliderObject target);
	void CalculateAABB(Vector3 p);

private:
	Vector3 size = { 0,0,0 };
	RectColliderObject colliderObject;
};

