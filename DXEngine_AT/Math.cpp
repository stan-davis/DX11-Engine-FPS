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

float Vector3::Distance(const Vector3& vec3) const
{
	float x_sqr = (vec3.x - x) * (vec3.x - x);
	float y_sqr = (vec3.y - y) * (vec3.y - y);
	float z_sqr = (vec3.z - z) * (vec3.z - z);

	return std::sqrtf(x_sqr + y_sqr + z_sqr);
}

float Vector3::Magnitude(const Vector3& vec3) const
{
	return (x * x) + (y * y) + (z * z);
}
