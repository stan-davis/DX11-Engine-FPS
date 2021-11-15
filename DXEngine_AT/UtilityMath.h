#pragma once

struct Vector3
{
	float x, y, z;

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vector3(const Vector3& vec3);

	Vector3 operator+(const Vector3& vec3) const;
	Vector3 operator-(const Vector3& vec3) const;
	Vector3 operator*(const Vector3& vec3) const;
	Vector3 operator/(const Vector3& vec3) const;

	Vector3 operator*(const float& f) const;
	Vector3 operator/(const float& f) const;

	float Magnitude() const;
	void Normalize();
};