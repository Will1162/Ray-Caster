#pragma once

struct Sphere
{
	Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);
	float rad;
	Colour col = Colour(0, 0, 0);

	Sphere(Vec3 pos, float rad, Colour col)
	{
		this->pos = pos;
		this->rad = rad;
		this->col = col;
	}
};