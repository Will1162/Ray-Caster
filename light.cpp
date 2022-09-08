#pragma once

struct Light
{
	Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);
	float i = 1.0f; // intensity

	Light(Vec3 pos, float i)
	{
		this->pos = pos;
		this->i = i;
	}
};