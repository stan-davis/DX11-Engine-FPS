#pragma once
#include <DirectXMath.h>
#include "Mesh.h"
#include "UtilityMath.h"
#include "Collider.h"

#include <memory>

namespace DX = DirectX;

class Entity
{
public:
	explicit Entity();
	explicit Entity(Mesh _mesh);

	~Entity() = default;

	virtual void Update();
	virtual void Draw();

	DirectX::XMMATRIX GetMatrix() { return matrix; }
	Vector3 GetTransform();
	Vector3 GetRotation();
	Vector3 GetScale();

	void Translate(Vector3 position);
	void Rotate(Vector3 axis, float angle);
	void Scale(Vector3 factor);

 	Mesh GetMesh() { return mesh; }
	Collider GetCollider() { return collider; }

	void SetCollider(Collider _collider) { collider = _collider; }

	std::string Tag() { return tag; }
	void SetTag(std::string _tag);

	void IsBillboard(bool value);
	void BillboardUpdate(Vector3 cameraPosition);

protected:
	void Init();

	DX::XMMATRIX matrix;

	DX::XMMATRIX translation;
	DX::XMMATRIX scale;
	DX::XMMATRIX rotation;

	Mesh mesh;
	Collider collider;

	std::string tag;
	bool billboard = false;
};

