#pragma once

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
			Vec3 s = sphereList[i].pos;
			Vec3 d = p1 - p0;

			float a = d.x * d.x + d.y * d.y + d.z * d.z;
			float b = 2 * d.x * (p0.x - s.x) + 2 * d.y * (p0.y - s.y) + 2 * d.z * (p0.z - s.z);
			float c = s.x * s.x + s.y * s.y + s.z * s.z + p0.x * p0.x + p0.y * p0.y + p0.z * p0.z + -2 * (s.x * p0.x + s.y * p0.y + s.z * p0.z) - sphereList[i].rad * sphereList[i].rad;
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

bool SpherePointInShadow(Vec3 spherePoint, Light light, Sphere sphereList[], int sphereCount, int ignoreIndex)
{
	float lightDist = (light.pos - spherePoint).Length();
	
	Vec3 intersection = Vec3(0.0f, 0.0f, 0.0f);
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