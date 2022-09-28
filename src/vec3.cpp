#include "vec3.hpp"

#include <math.h>

namespace WB_RT
{
	Vec3::Vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float Vec3::Length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 Vec3::Normalise()
	{
		float len = Length();
		return Vec3(x / len, y / len, z / len);
	}

	float Vec3::Dot(Vec3 other)
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vec3 Vec3::Cross(Vec3 other)
	{
		return Vec3(
			y * other.z - z * other.y,
			x * other.z - z * other.x,
			x * other.y - y * other.x
		);
	}

	float Vec3::AngleBetween(Vec3 other)
	{
		float lenProduct = Length() * other.Length();
		return acos(Dot(other) / lenProduct);
	}

	Vec3 operator+(Vec3 a, Vec3 b)
	{
		return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	Vec3 operator-(Vec3 a, Vec3 b)
	{
		return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	Vec3 operator*(Vec3 a, float b)
	{
		return Vec3(a.x * b, a.y * b, a.z * b);
	}
}