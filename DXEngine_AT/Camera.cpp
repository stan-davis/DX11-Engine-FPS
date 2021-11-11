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

DX::XMMATRIX Camera::GetCameraMatrix()
{
	rotation = DX::XMMatrixRotationRollPitchYaw(0, yaw, 0);

	camTarget = DX::XMVector3TransformCoord(worldForward, rotation);
	camTarget = DX::XMVector3Normalize(camTarget);

	DX::XMMATRIX rotateY = DX::XMMatrixRotationY(yaw);
	
	camUp = DX::XMVector3TransformCoord(camUp, rotateY);
	camForward = DX::XMVector3TransformCoord(worldForward, rotateY);

	DX::XMVECTOR move = DX::XMVectorSet(GetTranslation().x, 0, GetTranslation().z, 0);
	DX::XMVECTOR position = DX::XMVectorMultiply(camForward, move);
	camPosition = DX::XMVectorAdd(camPosition, position);

	Translate(Vector3(0, 0, 0));

	camTarget = DX::XMVectorAdd(camPosition, camTarget);

	viewMatrix = DX::XMMatrixLookAtLH(camPosition, camTarget, camUp);

	return matrix * viewMatrix * projectionMatrix;
}