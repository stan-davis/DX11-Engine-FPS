#include "Math.h"
#include <cmath>

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
