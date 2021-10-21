#pragma once

#include <DirectXMath.h>

namespace dx = DirectX;

class Camera
{
public:
	Camera(float fov, float aspectRatio, float near, float far);
	~Camera() = default;

	void Update();
	dx::XMMATRIX GetWorldProjectionMatrix();

	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z, float angle);

private:
	//World Matrices
	dx::XMMATRIX World;

	dx::XMMATRIX Rotation;
	dx::XMMATRIX Scale;
	dx::XMMATRIX Translation;

	//Camera
	dx::XMMATRIX camView;
	dx::XMMATRIX camProjection;

	dx::XMVECTOR camPosition;
	dx::XMVECTOR camTarget;
	dx::XMVECTOR camUp;
};

