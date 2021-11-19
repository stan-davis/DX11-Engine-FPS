#pragma once

#include "Entity.h"
#include "CameraComponent.h"

class Player : public Entity
{
public:
	Player(CameraComponent _camera);
	~Player() = default;

	void Update() override;

	void RotateYAW(float direction, float delta);
	void Move(Vector3 direction, float delta);

	Vector3 GetTarget() { return target; }
	void SetTarget(Vector3 t) { target = t; }

	Vector3 GetDirection();

	CameraComponent GetCamera() { return *camera; }

private:
	std::unique_ptr<CameraComponent> camera;

	Vector3 target = { 0,0,0 };

	const float m_velocity = 10;
	const float r_velocity = 4;
};

