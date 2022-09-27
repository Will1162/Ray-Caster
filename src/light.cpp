#include "light.hpp"

Light::Light(Vec3 pos, Colour col, float i)
{
	this->pos = pos;
	this->col = col;
	this->i = i;
}