#pragma once

#include <DirectXMath.h>
#include "Entity.h"

class Camera : public Entity
{
public:
	Camera(float fov, float aspectRatio, float _near, float _far);
	~Camera() = default;

	void RotateYAW(float value);

	DX::XMMATRIX GetCameraMatrix();
private:
	//Matrix
	DX::XMMATRIX viewMatrix = DX::XMMatrixIdentity();
	DX::XMMATRIX projectionMatrix = DX::XMMatrixIdentity();

	//World Matrix Defaults
	const DX::XMVECTOR worldRight = DX::XMVectorSet(1, 0, 0, 0);
	const DX::XMVECTOR worldForward = DX::XMVectorSet(0, 0, 1, 0);

	//Camera
	DX::XMVECTOR camUp = DX::XMVectorSet(0,1,0,0);
	DX::XMVECTOR camRight = DX::XMVectorSet(1, 0, 0, 0);
	DX::XMVECTOR camForward = DX::XMVectorSet(0, 0, 1, 0);

	DX::XMVECTOR camPosition;
	DX::XMVECTOR camTarget = DX::XMVectorSet(0, 0, 0, 0);;

	float yaw = 0;
};

