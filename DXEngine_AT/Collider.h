#pragma once
#include "UtilityMath.h"

class Collider
{
public:
	enum class ColliderShape
	{
		NONE,
		RECT,
		SPHERE
	};

	Collider() = default;
	Collider(Vector3 _position, Vector3 _size) : position(_position), size(_size), colliderShape(ColliderShape::RECT) {}
	Collider(Vector3 _position, float _radius) : position(_position), radius(_radius), colliderShape(ColliderShape::SPHERE) {}
	~Collider() = default;

	void UpdateCollider(Vector3 _position) { position = _position; }
	bool IsColliding(Collider collider);

	ColliderShape GetColliderType() { return colliderShape; }
	Vector3 GetSize() { return size; }
	float GetRadius() { return radius; }
	Vector3 GetPosition() { return position; }

private:
	ColliderShape colliderShape = ColliderShape::NONE;
	Vector3 size = { 0,0,0 };
	Vector3 position = { 0,0,0 };
	float radius = 0.0f;

	bool RectCollider(Collider& collider);
};

