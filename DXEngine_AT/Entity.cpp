#include "Entity.h"

Entity::Entity()
{
	Init();
}

Entity::Entity(Mesh _mesh) : mesh(_mesh)
{
	Init();
}

void Entity::Init()
{
	matrix = DX::XMMatrixIdentity();
	scale = DX::XMMatrixIdentity();
	rotation = DX::XMMatrixIdentity();
}

void Entity::Update()
{
	
}

void Entity::Draw()
{
	matrix = scale * rotation * translation;
}

Vector3 Entity::GetTranslation()
{
	Vector3 output = Vector3(0, 0, 0);
	DX::XMVECTOR sca;
	DX::XMVECTOR rot;
	DX::XMVECTOR vec;

	DX::XMMatrixDecompose(&sca, &rot, &vec, translation);
	
	output.x = DX::XMVectorGetX(vec);
	output.y = DX::XMVectorGetY(vec);
	output.z = DX::XMVectorGetZ(vec);

	return output;
}

Vector3 Entity::GetRotation()
{
	Vector3 output = Vector3(0, 0, 0);
	DX::XMVECTOR vec;

	DX::XMMatrixDecompose(nullptr, &vec, nullptr, translation);

	output.x = DX::XMVectorGetX(vec);
	output.y = DX::XMVectorGetY(vec);
	output.z = DX::XMVectorGetZ(vec);

	return output;
}

Vector3 Entity::GetScale()
{
	Vector3 output = Vector3(0, 0, 0);
	DX::XMVECTOR vec;

	DX::XMMatrixDecompose(&vec, nullptr, nullptr, translation);

	output.x = DX::XMVectorGetX(vec);
	output.y = DX::XMVectorGetY(vec);
	output.z = DX::XMVectorGetZ(vec);

	return output;
}

void Entity::Translate(Vector3 position)
{
	translation = DX::XMMatrixTranslation(position.x, position.y, position.z);
}

void Entity::Rotate(Vector3 axis, float angle)
{
	DX::XMVECTOR _axis = DX::XMVectorSet(axis.x, axis.y, axis.z, 0.0f);
	rotation = DX::XMMatrixRotationAxis(_axis, angle);
}

void Entity::Scale(Vector3 factor)
{
	scale = DX::XMMatrixScaling(factor.x, factor.y, factor.z);
}
