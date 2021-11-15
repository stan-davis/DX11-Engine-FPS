#include "Collider.h"

bool Collider::RectCollision(RectColliderObject o1, RectColliderObject o2)
{
	if (DX::XMVectorGetX(o1.maxVertex) > DX::XMVectorGetX(o2.minVertex))
		if (DX::XMVectorGetX(o1.minVertex) < DX::XMVectorGetX(o2.maxVertex))
			if (DX::XMVectorGetY(o1.maxVertex) > DX::XMVectorGetY(o2.minVertex))
				if (DX::XMVectorGetY(o1.minVertex) < DX::XMVectorGetY(o2.maxVertex))
					if (DX::XMVectorGetZ(o1.maxVertex) > DX::XMVectorGetZ(o2.minVertex))
						if (DX::XMVectorGetZ(o1.minVertex) < DX::XMVectorGetZ(o2.maxVertex))
							return true;

	return false;
}

void Collider::CalculateAABB(std::vector<DX::XMFLOAT3> verts, DX::XMMATRIX& worldSpace)
{
	vertices = verts;
	matrix = worldSpace;

	DX::XMFLOAT3 minVertex = DX::XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	DX::XMFLOAT3 maxVertex = DX::XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (int i = 0; i < 8; i++)
	{
		DX::XMVECTOR vert = DX::XMVectorSet(vertices[i].x, vertices[i].y, vertices[i].z, 0);
		vert = DX::XMVector3TransformCoord(vert, matrix);

		minVertex.x = std::min(minVertex.x, DX::XMVectorGetX(vert));
		minVertex.y = std::min(minVertex.y, DX::XMVectorGetY(vert));
		minVertex.z = std::min(minVertex.z, DX::XMVectorGetZ(vert));

		maxVertex.x = std::max(maxVertex.x, DX::XMVectorGetX(vert));
		maxVertex.y = std::max(maxVertex.y, DX::XMVectorGetY(vert));
		maxVertex.z = std::max(maxVertex.z, DX::XMVectorGetZ(vert));

		colliderObject.minVertex = DX::XMVectorSet(minVertex.x, minVertex.y, minVertex.z, 0);
		colliderObject.maxVertex = DX::XMVectorSet(maxVertex.x, maxVertex.y, maxVertex.z, 0);
	}
}
