#include "sphere.hpp"

Sphere::Sphere(Vec3 pos, float rad, Material mat)
{
	this->pos = pos;
	this->rad = rad;
	this->mat = mat;
}