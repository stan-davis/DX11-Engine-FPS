#pragma once

#include "Entity.h"

class Bullet : public Entity
{
public:
	Bullet() = default;
	~Bullet() = default;

	void Move(float delta);
	void SetMovement(Vector3 _direction);

private:
	const float m_velocity = 60;
	Vector3 direction = { 0,0,0 };
};

