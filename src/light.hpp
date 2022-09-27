#ifndef WB_RT_LIGHT_HPP
#define WB_RT_LIGHT_HPP

#include "colour.hpp"
#include "vec3.hpp"

class Light
{
	public:
		Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);
		Colour col = Colour(0, 0, 0);
		float i;

		Light(Vec3 pos, Colour col, float i);
};

#endif