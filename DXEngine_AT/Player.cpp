#include "Player.h"

Player::Player(CameraComponent _camera)
{
	Init();

	camera = std::make_unique<CameraComponent>(_camera);
}

void Player::Update()
{
	camera->Update(this->GetTransform());

	collider.CalculateAABB(GetTransform());
}

void Player::RotateYAW(float direction, float delta)
{
	camera->Rotate((r_velocity * direction) * delta);
}

void Player::Move(Vector3 direction, float delta)
{
	//Next potential position
	float x = GetTransform().x + (m_velocity * direction.x) * sinf(camera->GetYAW()) * delta;
	float z = GetTransform().z + (m_velocity * direction.z) * cosf(camera->GetYAW()) * delta;

	target = { x, 0, z };
}

Vector3 Player::GetDirection()
{
	Vector3 v = { 0,0,0 };

	//Save facing direction for usage later
	float xf = sinf(camera->GetYAW());
	float zf = cosf(camera->GetYAW());

	v = { xf, 0, zf };
	v.Normalize();

	return v;
}
