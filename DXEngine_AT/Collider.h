#pragma once
#include "UtilityMath.h"
#include <DirectXMath.h>
#include <vector>

namespace DX = DirectX;

class Collider
{
public:
	struct RectColliderObject
	{
		DX::XMVECTOR& minVertex;
		DX::XMVECTOR& maxVertex;
	};

	Collider() = default;
	~Collider() = default;

	bool RectCollision(RectColliderObject o1, RectColliderObject o2);
	void CalculateAABB(std::vector<DX::XMFLOAT3> verts, DX::XMMATRIX& worldSpace);

private:
	std::vector<DX::XMFLOAT3> vertices;
	DX::XMMATRIX matrix;
	RectColliderObject colliderObject;
};

