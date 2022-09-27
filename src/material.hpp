#ifndef WB_RT_MATERIAL_HPP
#define WB_RT_MATERIAL_HPP

#include "colour.hpp"

class Material
{
	public:
		Colour col = Colour(0, 0, 0);
		float diff; // diffuse
		float spec; // specular
		float refl; // reflectivity
		float amb;  // ambient

		Material(Colour col, float diff, float spec, float refl, float amb);
};

#endif