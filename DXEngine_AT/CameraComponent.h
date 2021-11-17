#pragma once
#include <DirectXMath.h>
#include "UtilityMath.h"

namespace DX = DirectX;

class CameraComponent
{
public:
	CameraComponent(float fov, float aspectRatio, float _near, float _far);
	~CameraComponent() = default;

	void Update(Vector3 transform);
	void Rotate(float value);

	float GetYAW() { return yaw; }
	DX::XMMATRIX GetCameraMatrix();

private:
	//Matrices
	DX::XMMATRIX rotationMatrix = DX::XMMatrixIdentity();
	DX::XMMATRIX viewMatrix = DX::XMMatrixIdentity();
	DX::XMMATRIX projectionMatrix = DX::XMMatrixIdentity();

	//World Constants
	const DX::XMVECTOR worldForward = DX::XMVectorSet(0, 0, 1, 0);
	const DX::XMVECTOR camUp = DX::XMVectorSet(0, 1, 0, 0);

	//Directional
	DX::XMVECTOR camPosition = DX::XMVectorSet(0, 0, 0, 0);
	DX::XMVECTOR camTarget = DX::XMVectorSet(0, 0, 0, 0);

	float yaw = 0;
};

