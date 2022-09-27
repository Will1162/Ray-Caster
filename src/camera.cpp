#pragma once

#include "vec3.cpp"

struct Camera
{
	Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);
	Vec3 rot = Vec3(0.0f, 0.0f, 0.0f);
	float fov;
	float focalLength;

	Camera(Vec3 pos, Vec3 rot, float fov)
	{
		this->pos = pos;
		this->rot = rot;
		this->fov = fov;
		focalLength = 1.0f / tan(fov * atan(1) / 90.0f);
	}
};