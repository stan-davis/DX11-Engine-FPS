#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float near, float far)
{
	//Set up camera TESTING ONLY
	camPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.001f, 0.0f);
	camTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
	camProjection = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, near, far);

	//Required to draw objects in world space
	Translate(0.0f, 0.0f, 0.0f);
	Rotate(0.0f, 1.0f, 0.0f, 0.0f);
}

void Camera::Update()
{
	world = DirectX::XMMatrixIdentity();
	world = translation * rotation;
}

dx::XMMATRIX Camera::GetWorldProjectionMatrix()
{
	return world * camView * camProjection;
}

void Camera::Translate(float x, float y, float z)
{
	translation = DirectX::XMMatrixTranslation(x, y, z);
}

void Camera::Rotate(float x, float y, float z, float angle)
{
	dx::XMVECTOR axis = dx::XMVectorSet(x, y, z, 0.0f);
	rotation = dx::XMMatrixRotationAxis(axis, angle);
}
