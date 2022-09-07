#pragma once

#include "vec3.cpp"

struct Camera
{
	Vec3 pos;
	float fov;
	float focalLength;

	Camera()
	{
		pos = Vec3(0.0, 0.0, 0.0);
	}

	Camera(Vec3 pos)
	{
		this->pos = pos;
	}

	Camera(Vec3 pos, float fov)
	{
		this->pos = pos;
		this->fov = fov;
		focalLength = 1.0 / tan(fov * atan(1) / 90.0);
	}
};