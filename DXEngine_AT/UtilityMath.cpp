#include "UtilityMath.h"
#include <cmath>

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
