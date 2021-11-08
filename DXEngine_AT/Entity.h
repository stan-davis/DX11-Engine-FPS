#pragma once
#include <DirectXMath.h>
#include "Mesh.h"
#include "Math.h"

class Entity
{
public:
	Entity() = default;
	Entity(Mesh _mesh, Vector3 _position);

	~Entity() = default;

	Vector3 GetPosition() { return position; }
	void SetPosition(Vector3 _position);
	void SetRotation(Vector3 axis, float angle);

	DirectX::XMMATRIX GetMatrix() { return matrix; }
	Mesh GetMesh() { return mesh; }

private:
	DirectX::XMMATRIX matrix;
	Vector3 position = { 0, 0, 0 };
	Mesh mesh;
};

