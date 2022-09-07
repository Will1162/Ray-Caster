#pragma once

#include "vec3.cpp"

struct Camera
{
	Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);
	float fov;
	float focalLength;

	Camera(Vec3 pos, float fov)
	{
		this->pos = pos;
		this->fov = fov;
		focalLength = 1.0f / tan(fov * atan(1) / 90.0f);
	}
};