#pragma once

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

	float length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 normalise()
	{
		float len = length();
		return Vec3(x / len, y / len, z / len);
	}

	float dotProduct(Vec3 other)
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vec3 crossProduct(Vec3 other)
	{
		return Vec3(
			y * other.z - z * other.y,
			x * other.z - z * other.x,
			x * other.y - y * other.x
		);
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