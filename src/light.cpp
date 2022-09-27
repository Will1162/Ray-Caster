#pragma once

struct Light
{
	Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);
	Colour col = Colour(0, 0, 0);
	float i = 1.0f; // intensity

	Light(Vec3 pos, Colour col, float i)
	{
		this->pos = pos;
		this->col = col;
		this->i = i;
	}
};