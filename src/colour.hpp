#ifndef WB_RT_COLOUR_HPP
#define WB_RT_COLOUR_HPP

class Colour
{
	public:
		int r, g, b;

		Colour(int r, int g, int b);
		Colour Lerp(Colour c, float t);
		void Clamp();

		friend Colour operator+(Colour a, Colour b);
		friend Colour operator+(Colour a, float b);
		friend Colour operator*(Colour a, Colour b);
		friend Colour operator*(Colour a, float b);
		friend Colour operator/(Colour a, float b);
};

#endif