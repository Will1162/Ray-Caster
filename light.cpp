#pragma once

struct Light
{
	Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);

	Light(Vec3 pos)
	{
		this->pos = pos;
	}
};