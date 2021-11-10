#pragma once
#include <DirectXMath.h>
#include "Mesh.h"
#include "Math.h"

#include <memory>

class Entity
{
public:
	Entity();
	Entity(Mesh _mesh);

	~Entity() = default;

	void Update();
	void Draw();

	DirectX::XMMATRIX GetMatrix() { return matrix; }

	void Translate(Vector3 position);
	void Rotate(Vector3 axis, float angle);
	void Scale(Vector3 factor);
	
	Mesh GetMesh() { return mesh; }

private:
	void Init();

	DirectX::XMMATRIX matrix;

	DirectX::XMMATRIX translation;
	DirectX::XMMATRIX scale;
	DirectX::XMMATRIX rotation;

	Mesh mesh;
};

