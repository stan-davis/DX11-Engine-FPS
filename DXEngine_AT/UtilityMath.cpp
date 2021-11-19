#include "UtilityMath.h"
#include <cmath>
#include <algorithm>

Vector3::Vector3(const Vector3& vec3)
{
	x = vec3.x; 
	y = vec3.y; 
	z = vec3.z;
}

Vector3 Vector3::operator+(const Vector3& vec3) const
{
	return Vector3(x + vec3.x, y + vec3.y, z + vec3.z);
}

Vector3 Vector3::operator-(const Vector3& vec3) const
{
	return Vector3(x - vec3.x, y - vec3.y, z - vec3.z);
}

Vector3 Vector3::operator*(const Vector3& vec3) const
{
	return Vector3(x * vec3.x, y * vec3.y, z * vec3.z);
}

Vector3 Vector3::operator/(const Vector3& vec3) const
{
	return Vector3(x / vec3.x, y / vec3.y, z / vec3.z);
}

Vector3 Vector3::operator*(const float& f) const
{
	return Vector3(x * f, y * f, z * f);
}

Vector3 Vector3::operator/(const float& f) const
{
	return Vector3(x / f, y / f, z / f);
}


float Vector3::Magnitude() const
{
	float _x = x * x;
	float _y = y * y;
	float _z = z * z;

	return std::sqrtf(_x + _y + _z);
}

void Vector3::Normalize()
{
	if (Magnitude() <= 0)
		return;

	x /= Magnitude();
	y /= Magnitude();
	z /= Magnitude();
}

void Vector3::Floor()
{
	x = std::floor(x);
	y = std::floor(y);
	z = std::floor(z);
}

Vector3 Vector3::Min(const Vector3& vec3)
{
	return Vector3(std::min(x, vec3.x), std::min(y, vec3.y), std::min(z, vec3.z));
}

Vector3 Vector3::Max(const Vector3& vec3)
{
	return Vector3(std::max(x, vec3.x), std::max(y, vec3.y), std::max(z, vec3.z));
}
