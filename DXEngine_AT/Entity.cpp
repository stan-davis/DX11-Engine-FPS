#include "Entity.h"

Entity::Entity(Mesh _mesh, Vector3 _position) : mesh(_mesh)
{
	matrix = DirectX::XMMatrixIdentity();
	SetPosition(_position);
}

void Entity::SetPosition(Vector3 _position)
{
	position = _position;
	matrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

void Entity::SetRotation(Vector3 axis, float angle)
{
	DirectX::XMVECTOR _axis = DirectX::XMVectorSet(axis.x, axis.y, axis.z, 0);
	matrix = DirectX::XMMatrixRotationAxis(_axis, angle);
}
