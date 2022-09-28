#include "light.hpp"

namespace WB_RT
{
	Light::Light(Vec3 pos, Colour col, float i)
	{
		this->pos = pos;
		this->col = col;
		this->i = i;
	}
}