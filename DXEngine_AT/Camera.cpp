#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float near, float far)
{
	//Set up camera TESTING ONLY
	camPosition = DirectX::XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	camTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
	camProjection = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, near, far);
}

void Camera::Update()
{
	World = DirectX::XMMatrixIdentity();
	World = Translation * Rotation;
}

dx::XMMATRIX Camera::GetWorldProjectionMatrix()
{
	return World * camView * camProjection;
}

void Camera::Translate(float x, float y, float z)
{
	Translation = DirectX::XMMatrixTranslation(x, y, z);
}

void Camera::Rotate(float x, float y, float z, float angle)
{
	dx::XMVECTOR axis = dx::XMVectorSet(x, y, z, 0.0f);
	Rotation = dx::XMMatrixRotationAxis(axis, angle);
}
