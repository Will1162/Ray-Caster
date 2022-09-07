#pragma once

int ClampRGBVal(int i)
{
	if (i > 255)
		i = 255;
	if (i < 0)
		i = 0;
	return i;
}

struct Colour
{
	int r;
	int g;
	int b;

	Colour()
	{
		r = 0;
		g = 0;
		b = 0;
	}

	Colour(int i)
	{
		r = ClampRGBVal(i);
		g = ClampRGBVal(i);
		b = ClampRGBVal(i);
	}

	Colour(int r, int g, int b)
	{
		this->r = ClampRGBVal(r);
		this->g = ClampRGBVal(g);
		this->b = ClampRGBVal(b);
	}

	Colour Lerp(Colour c, float t)
	{
		return Colour(r + (c.r - r) * t, g + (c.g - g) * t, b + (c.b - b) * t);
	}
};

Colour ClampRGB(Colour col)
{
	if (col.r > 255)
		col.r = 255;
	if (col.g > 255)
		col.g = 255;
	if (col.b > 255)
		col.b = 255;
	if (col.r < 0)
		col.r = 0;
	if (col.g < 0)
		col.g = 0;
	if (col.b < 0)
		col.b = 0;
	return col;
}

Colour operator+(Colour a, Colour b)
{
	return Colour(a.r + b.r, a.g + b.g, a.b + b.b);
}

Colour operator+(Colour a, float b)
{
	return Colour(a.r + b, a.g + b, a.b + b);
}

Colour operator-(Colour a, Colour b)
{
	return Colour(a.r - b.r, a.g - b.g, a.b - b.b);
}

Colour operator-(Colour a, float b)
{
	return Colour(a.r - b, a.g - b, a.b - b);
}

Colour operator*(Colour a, Colour b)
{
	return Colour(a.r * b.r, a.g * b.g, a.b * b.b);
}

Colour operator*(Colour a, float b)
{
	return Colour(a.r * b, a.g * b, a.b * b);
}

Colour operator/(Colour a, Colour b)
{
	return Colour(a.r / b.r, a.g / b.g, a.b / b.b);
}

Colour operator/(Colour a, float b)
{
	return Colour(a.r / b, a.g / b, a.b / b);
}