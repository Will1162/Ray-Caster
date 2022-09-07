#pragma once

#include <math.h>
#include <SFML/Graphics.hpp>

#include "light.cpp"
#include "sphere.cpp"
#include "vec3.cpp"

void DrawPixel(int x, int y, sf::Uint8 *pixels, int windowWidth, int windowHeight, int r, int g, int b)
{
	pixels[(y * windowWidth + x) * 4 + 0] = (unsigned char)r;
	pixels[(y * windowWidth + x) * 4 + 1] = (unsigned char)g;
	pixels[(y * windowWidth + x) * 4 + 2] = (unsigned char)b;
	pixels[(y * windowWidth + x) * 4 + 3] = 255;
}

void DrawPixel(int x, int y, sf::Uint8 *pixels, int windowWidth, int windowHeight, Colour(col))
{
	pixels[(y * windowWidth + x) * 4 + 0] = (unsigned char)col.r;
	pixels[(y * windowWidth + x) * 4 + 1] = (unsigned char)col.g;
	pixels[(y * windowWidth + x) * 4 + 2] = (unsigned char)col.b;
	pixels[(y * windowWidth + x) * 4 + 3] = 255;
}

bool RaySphereIntersection(Vec3 p0, Vec3 p1, Sphere sphereList[], int sphereCount, Vec3 &intersection, int &sphereHitIndex, int ignoreIndex = -1)
{
	float closestT = 1000000.0f;
	for (int i = 0; i < sphereCount; i++)
	{
		if (i != ignoreIndex)
		{
			Vec3 d = p1 - p0;
			float a = d.x * d.x + d.y * d.y + d.z * d.z;
			float b = 2 * d.x * (p0.x - sphereList[i].x) + 2 * d.y * (p0.y - sphereList[i].y) + 2 * d.z * (p0.z - sphereList[i].z);
			float c = sphereList[i].x * sphereList[i].x + sphereList[i].y * sphereList[i].y + sphereList[i].z * sphereList[i].z + p0.x * p0.x + p0.y * p0.y + p0.z * p0.z + -2 * (sphereList[i].x * p0.x + sphereList[i].y * p0.y + sphereList[i].z * p0.z) - sphereList[i].rad * sphereList[i].rad;

			float discriminant = b * b - 4 * a * c;

			if (discriminant >= 0)
			{
				float t = (-b - sqrt(discriminant)) / (2 * a);

				if (t < closestT)
				{
					closestT = t;
					sphereHitIndex = i;
					intersection = p0 + d * t;
				}
			}
		}
	}

	if (closestT < 1000000.0f)
	{
		return true;
	}
	return false;
}

float SphereDistanceToLight(Sphere sphere, Light light)
{
	return sqrt((sphere.x - light.x) * (sphere.x - light.x) + (sphere.y - light.y) * (sphere.y - light.y) + (sphere.z - light.z) * (sphere.z - light.z)) - sphere.rad;
}

bool SpherePointInShadow(Vec3 spherePoint, Light light, Sphere sphereList[], int sphereCount, int ignoreIndex)
{
	float lightDist = (light.pos - spherePoint).Length();
	
	Vec3 intersection;
	int sphereHitIndex;
	if (RaySphereIntersection(spherePoint, light.pos, sphereList, sphereCount, intersection, sphereHitIndex, ignoreIndex))
	{
		if ((light.pos - intersection).Length() < lightDist)
		{
			return true;
		}
	}

	return false;
}