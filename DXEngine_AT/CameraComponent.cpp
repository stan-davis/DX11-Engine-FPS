#include "CameraComponent.h"

CameraComponent::CameraComponent(float fov, float aspectRatio, float _near, float _far)
{
	float rFov = fov * (DirectX::XM_PI / 180);
	projectionMatrix = DX::XMMatrixPerspectiveFovLH(rFov, aspectRatio, _near, _far);
}

void CameraComponent::Update(Vector3 transform)
{
	//Set rotation based of YAW value
	rotationMatrix = DX::XMMatrixRotationRollPitchYaw(0, yaw, 0);

	//Set Camera Target to Z Forward & normalize
	camTarget = DX::XMVector3TransformCoord(worldForward, rotationMatrix);
	camTarget = DX::XMVector3Normalize(camTarget);

	//Convert transform to XMVECTOR and set matrices
	camPosition = DX::XMVectorSet(transform.x, transform.y, transform.z, 0);
	camTarget = DX::XMVectorAdd(camPosition, camTarget);
	viewMatrix = DX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

void CameraComponent::Rotate(float value)
{
	yaw += value;
}

DX::XMMATRIX CameraComponent::GetCameraMatrix()
{
	return viewMatrix * projectionMatrix;
}
