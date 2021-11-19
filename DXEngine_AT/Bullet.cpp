#include "Bullet.h"

void Bullet::Move(float delta)
{
	float x = GetTransform().x + (m_velocity * direction.x) * delta;
	float z = GetTransform().z + (m_velocity * direction.z) * delta;

	Translate({ x, 0, z });
}

void Bullet::SetMovement(Vector3 _direction)
{
	direction = _direction;
}
