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
	matrix = DirectX::XMMatrixIdentity();
	scale = DirectX::XMMatrixIdentity();
	rotation = DirectX::XMMatrixIdentity();
}

void Entity::Update()
{
	
}

void Entity::Draw()
{
	matrix = scale * rotation * translation;
}

void Entity::Translate(Vector3 position)
{
	translation = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

void Entity::Rotate(Vector3 axis, float angle)
{
	DirectX::XMVECTOR _axis = DirectX::XMVectorSet(axis.x, axis.y, axis.z, 0.0f);
	rotation = DirectX::XMMatrixRotationAxis(_axis, angle);
}

void Entity::Scale(Vector3 factor)
{
	scale = DirectX::XMMatrixScaling(factor.x, factor.y, factor.z);
}
