#pragma once

#include "vec3.cpp"

struct Light
{
	float x;
	float y;
	float z;
	float i;
	Vec3 pos;

	Light()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		i = 1.0f;
		pos = Vec3(x, y, z);
	}

	Light(float x, float y, float z, float i)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->i = i;
		pos = Vec3(x, y, z);
	}

	Light(Vec3 pos, float i)
	{
		this->x = pos.x;
		this->y = pos.y;
		this->z = pos.z;
		this->i = i;
		this->pos = Vec3(x, y, z);
	}
};