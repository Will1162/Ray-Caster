#pragma once

#include "colour.cpp"

struct Material
{
	Colour col = Colour(0, 0, 0);
	float diff; // diffuse
	float spec; // specular
	float refl; // reflectivity
	float amb;  // ambient

	Material(Colour col, float diff, float spec, float refl, float amb)
	{
		this->col = col;
		this->diff = diff;
		this->spec = spec;
		this->refl = refl;
		this->amb = amb;
	}
};