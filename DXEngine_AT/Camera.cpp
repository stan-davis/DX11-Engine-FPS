#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float near, float far)
{
	world = DirectX::XMMatrixIdentity();

	//Set up camera
	camPosition = DX::XMVectorSet(0.0f, 0.0f, 0.001f, 0.0f);
	camTarget = DX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	camView = DX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
	camProjection = DX::XMMatrixPerspectiveFovLH(fov, aspectRatio, near, far);

	//Required to draw objects in world space
	Translate(0.0f, 0.0f, 0.0f);
	Rotate(0.0f, 1.0f, 0.0f, 0.0f);
}

void Camera::Update()
{
	world = translation * rotation;
}

DX::XMMATRIX Camera::GetWorldProjectionMatrix()
{
	return world * camView * camProjection;
}

void Camera::Translate(float x, float y, float z)
{
	translation = DirectX::XMMatrixTranslation(x, y, z);
}

void Camera::Rotate(float x, float y, float z, float angle)
{
	DX::XMVECTOR axis = DX::XMVectorSet(x, y, z, 0.0f);
	rotation = DX::XMMatrixRotationAxis(axis, angle);
}
