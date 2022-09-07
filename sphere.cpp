#pragma once

struct Sphere
{
	Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);
	float rad;
	Material mat = Material(Colour(0, 0, 0), 0.0f, 0.0f, 0.0f, 0.0f);

	Sphere(Vec3 pos, float rad, Material mat)
	{
		this->pos = pos;
		this->rad = rad;
		this->mat = mat;
	}
};