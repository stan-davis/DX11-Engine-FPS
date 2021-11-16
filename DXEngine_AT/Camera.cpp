#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float _near, float _far)
{
	Init();
	float rFov = fov * (DirectX::XM_PI / 180);
	projectionMatrix = DX::XMMatrixPerspectiveFovLH(rFov, aspectRatio, _near, _far);
}

void Camera::RotateYAW(float direction, float delta)
{
	yaw += (r_velocity * direction) * delta;
}

void Camera::MoveForward(float direction, float delta)
{
	forward += (m_velocity * direction) * delta;
}

void Camera::Update()
{
	//Camera target is set to world forward position 
	rotation = DX::XMMatrixRotationRollPitchYaw(0, yaw, 0);

	camTarget = DX::XMVector3TransformCoord(worldForward, rotation);
	camTarget = DX::XMVector3Normalize(camTarget);

	DX::XMMATRIX rotateY = DX::XMMatrixRotationY(yaw);

	camUp = DX::XMVector3TransformCoord(camUp, rotateY);
	camForward = DX::XMVector3TransformCoord(worldForward, rotateY);

	//Move position when forward is pressed
	DX::XMVECTOR move = DX::XMVectorSet(forward, 0, forward, 0);
	DX::XMVECTOR position = DX::XMVectorMultiply(camForward, move);
	Vector3 position_vector = Vector3(DX::XMVectorGetX(position), DX::XMVectorGetY(position), DX::XMVectorGetZ(position));
	Translate(GetTransform() + position_vector);
	forward = 0;

	//Convert transform to XMVECTOR
	camPosition = DX::XMVectorSet(GetTransform().x, GetTransform().y, GetTransform().z, 0);
	camTarget = DX::XMVectorAdd(camPosition, camTarget);
	viewMatrix = DX::XMMatrixLookAtLH(camPosition, camTarget, camUp);

	collider.CalculateAABB(GetTransform());
}

DX::XMMATRIX Camera::GetCameraMatrix()
{
	return matrix * viewMatrix * projectionMatrix;
}