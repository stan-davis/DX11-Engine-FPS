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

void Entity::MakeBillboard()
{
	billboard = true;
}

void Entity::SetCameraDistance(Vector3 distance)
{
	cameraDistance = distance;
}

void Entity::MakeStatic()
{
	c_static = true;
	collider.CalculateAABB(this->GetTransform());
}

void Entity::SetActive(bool value)
{
	active = value;
	
	//Remove colliders and save a copy of original in-case
	if (!active)
	{
		inactiveCollider = collider;
		collider = Collider(); //Blank collider
	}
	else
	{
		collider = inactiveCollider;
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
	if (billboard)
	{
		float angle = std::atan2f(cameraDistance.x, cameraDistance.z);
		Rotate({ 0,1,0 }, angle);
	}

	if(!c_static) collider.CalculateAABB(this->GetTransform());
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
