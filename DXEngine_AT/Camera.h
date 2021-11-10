#pragma once

#include <DirectXMath.h>

namespace DX = DirectX;

class Camera
{
public:
	Camera(float fov, float aspectRatio, float near, float far);
	~Camera() = default;

	void Update();
	DX::XMMATRIX GetWorldProjectionMatrix();

	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z, float angle);

private:
	//World Matrices
	DX::XMMATRIX world;

	DX::XMMATRIX rotation;
	DX::XMMATRIX scale;
	DX::XMMATRIX translation;

	//Camera
	DX::XMMATRIX camView;
	DX::XMMATRIX camProjection;

	DX::XMVECTOR camPosition;
	DX::XMVECTOR camTarget;
	DX::XMVECTOR camUp;
};

