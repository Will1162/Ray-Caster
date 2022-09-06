#pragma once

#include <math.h>

struct Vec3
{
	float x;
	float y;
	float z;

	Vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float Length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 Normalise()
	{
		float len = Length();
		return Vec3(x / len, y / len, z / len);
	}

	float Dot(Vec3 other)
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vec3 Cross(Vec3 other)
	{
		return Vec3(
			y * other.z - z * other.y,
			x * other.z - z * other.x,
			x * other.y - y * other.x
		);
	}

	float AngleBetween(Vec3 other)
	{
		float lenProduct = Length() * other.Length();
		return acos(Dot(other) / lenProduct);
	}
};

Vec3 operator+(Vec3 a, Vec3 b)
{
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 operator+(Vec3 a, float b)
{
	return Vec3(a.x + b, a.y + b, a.z + b);
}

Vec3 operator-(Vec3 a, Vec3 b)
{
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 operator-(Vec3 a, float b)
{
	return Vec3(a.x - b, a.y - b, a.z - b);
}

Vec3 operator*(Vec3 a, Vec3 b)
{
	return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vec3 operator*(Vec3 a, float b)
{
	return Vec3(a.x * b, a.y * b, a.z * b);
}

Vec3 operator/(Vec3 a, Vec3 b)
{
	return Vec3(a.x / b.x, a.y / b.y, a.z / b.z);
}

Vec3 operator/(Vec3 a, float b)
{
	return Vec3(a.x / b, a.y / b, a.z / b);
}

bool operator==(Vec3 a, Vec3 b)
{
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

bool operator!=(Vec3 a, Vec3 b)
{
	return (a.x != b.x) && (a.y != b.y) && (a.z != b.z);
}