#ifndef WB_RT_SPHERE
#define WB_RT_SPHERE

#include "vec3.cpp"

struct sphere
{
	float x;
	float y;
	float z;
	float r;

	sphere()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		r = 1.0f;
	}

	sphere(float x, float y, float z, float r)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->r = r;
	}

	sphere(vec3 pos, float r)
	{
		this->x = pos.x;
		this->y = pos.y;
		this->z = pos.z;
		this->r = r;
	}
};

#endif