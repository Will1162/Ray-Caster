#include "colour.hpp"

Colour::Colour(int r, int g, int b)
{
	this->r = r;
	this->g = g;
	this->b = b;
	Clamp();
}

Colour Colour::Lerp(Colour c, float t)
{
	return Colour(r + (c.r - r) * t, g + (c.g - g) * t, b + (c.b - b) * t);
}

void Colour::Clamp()
{
	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;
	if (r < 0) r = 0;
	if (g < 0) g = 0;
	if (b < 0) b = 0;
}

Colour operator+(Colour a, Colour b)
{
	return Colour(a.r + b.r, a.g + b.g, a.b + b.b);
}

Colour operator+(Colour a, float b)
{
	return Colour(a.r + b, a.g + b, a.b + b);
}

Colour operator*(Colour a, Colour b)
{
	a = a / 255.0f;
	return Colour(a.r * b.r, a.g * b.g, a.b * b.b);
}

Colour operator*(Colour a, float b)
{
	return Colour(a.r * b, a.g * b, a.b * b);
}

Colour operator/(Colour a, float b)
{
	return Colour(a.r / b, a.g / b, a.b / b);
}