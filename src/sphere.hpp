#ifndef WB_RT_SPHERE_HPP
#define WB_RT_SPHERE_HPP

#include "material.hpp"
#include "vec3.hpp"

namespace WB_RT
{
	class Sphere
	{
		public:
			Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);
			float rad;
			Material mat = Material(Colour(0, 0, 0), 0.0f, 0.0f, 0.0f, 0.0f);

			Sphere(Vec3 pos, float rad, Material mat);
	};
}

#endif