#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float _near, float _far)
{
	Init();
	projectionMatrix = DX::XMMatrixPerspectiveFovLH(fov, aspectRatio, _near, _far);
}

void Camera::RotateYAW(float value)
{
	yaw += value;
}

void Camera::MoveForward(float value)
{
	forward += value;
}

DX::XMMATRIX Camera::GetCameraMatrix()
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

	return matrix * viewMatrix * projectionMatrix;
}