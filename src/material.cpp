#include "material.hpp"

Material::Material(Colour col, float diff, float spec, float refl, float amb)
{
	this->col = col;
	this->diff = diff;
	this->spec = spec;
	this->refl = refl;
	this->amb = amb;
}