#pragma once

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
		r = i;
		g = i;
		b = i;
	}

	Colour(int r, int g, int b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
};