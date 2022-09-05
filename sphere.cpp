#pragma once

#include "colour.cpp"
#include "vec3.cpp"

struct Sphere
{
	float x;
	float y;
	float z;
	float rad;
	Vec3 pos;
	Colour col;

	Sphere()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		rad = 1.0f;
		pos = Vec3(x, y, z);
		col = Colour(255, 255, 255);
	}

	Sphere(float x, float y, float z, float rad, int r, int g, int b)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->rad = rad;
		pos = Vec3(x, y, z);
		col = Colour(r, g, b);
	}

	Sphere(float x, float y, float z, float rad, Colour col)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->rad = rad;
		pos = Vec3(x, y, z);
		this->col = col;
	}

	Sphere(Vec3 pos, float rad, int r, int g, int b)
	{
		this->x = pos.x;
		this->y = pos.y;
		this->z = pos.z;
		this->rad = rad;
		pos = Vec3(x, y, z);
		col = Colour(r, g, b);
	}

	Sphere(Vec3 pos, float rad, Colour col)
	{
		this->x = pos.x;
		this->y = pos.y;
		this->z = pos.z;
		this->rad = rad;
		pos = Vec3(x, y, z);
		this->col = col;
	}
};