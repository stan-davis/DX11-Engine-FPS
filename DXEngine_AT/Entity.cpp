#include "Entity.h"
#include <cmath>

Entity::Entity()
{
	Init();
}

Entity::Entity(Mesh _mesh) : mesh(_mesh)
{
	Init();
}

void Entity::SetTag(std::string _tag)
{
	tag = _tag;
}

void Entity::IsBillboard(bool value)
{
	billboard = value;
}

void Entity::BillboardUpdate(Vector3 cameraPosition)
{
	if (billboard)
	{
		Vector3 distance = GetTransform() - cameraPosition;
		float angle = std::atan2f(distance.x, distance.z) * (180 / DX::XM_PI);
		angle *= 0.0174532925f; //the magic number
		Rotate({ 0,1,0 }, angle);
	}
}

void Entity::Init()
{
	matrix = DX::XMMatrixIdentity();
	scale = DX::XMMatrixIdentity();
	rotation = DX::XMMatrixIdentity();
}

void Entity::Update()
{
	collider.CalculateAABB(this->GetTransform());
}

void Entity::Draw()
{
	matrix = scale * rotation * translation;
}

Vector3 Entity::GetTransform()
{
	Vector3 output = { 0,0,0 };

	output.x = DX::XMVectorGetX(translation.r[3]);
	output.y = DX::XMVectorGetY(translation.r[3]);
	output.z = DX::XMVectorGetZ(translation.r[3]);

	return output;
}

Vector3 Entity::GetRotation()
{
	Vector3 output = { 0,0,0 };

	output.x = DX::XMVectorGetX(translation.r[2]);
	output.y = DX::XMVectorGetY(translation.r[2]);
	output.z = DX::XMVectorGetZ(translation.r[2]);

	return output;
}

Vector3 Entity::GetScale()
{
	Vector3 output = { 0,0,0 };

	output.x = DX::XMVectorGetX(translation.r[1]);
	output.y = DX::XMVectorGetY(translation.r[1]);
	output.z = DX::XMVectorGetZ(translation.r[1]);

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
