#ifndef WB_RT_VEC3
#define WB_RT_VEC3

struct vec3
{
	float x;
	float y;
	float z;

	vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

vec3 operator+(vec3 a, vec3 b)
{
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 operator-(vec3 a, vec3 b)
{
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

#endif