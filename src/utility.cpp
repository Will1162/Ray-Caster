#include "utility.hpp"

namespace WB_RT
{
	void DrawPixel(int x, int y, Colour(col))
	{
		PIXELS[(y * WINDOW_WIDTH + x) * 4 + 0] = (unsigned char)col.r;
		PIXELS[(y * WINDOW_WIDTH + x) * 4 + 1] = (unsigned char)col.g;
		PIXELS[(y * WINDOW_WIDTH + x) * 4 + 2] = (unsigned char)col.b;
		PIXELS[(y * WINDOW_WIDTH + x) * 4 + 3] = 255;
	}

	bool RaySphereIntersection(Vec3 p0, Vec3 p1, Vec3 &intersection, int &sphereHitIndex, int ignoreIndex = -1)
	{
		float closestT = 1000000.0f;
		for (int i = 0; i < SPHERE_COUNT; i++)
		{
			if (i != ignoreIndex)
			{
				Vec3 s = SPHERE_LIST[i].pos;
				Vec3 d = p1 - p0;

				float a = d.x * d.x + d.y * d.y + d.z * d.z;
				float b = 2 * d.x * (p0.x - s.x) + 2 * d.y * (p0.y - s.y) + 2 * d.z * (p0.z - s.z);
				float c = s.x * s.x + s.y * s.y + s.z * s.z + p0.x * p0.x + p0.y * p0.y + p0.z * p0.z + -2 * (s.x * p0.x + s.y * p0.y + s.z * p0.z) - SPHERE_LIST[i].rad * SPHERE_LIST[i].rad;
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

	bool SpherePointInShadow(Vec3 spherePoint, Light light, int ignoreIndex)
	{
		float lightDist = (light.pos - spherePoint).Length();
		
		Vec3 intersection = Vec3(0.0f, 0.0f, 0.0f);
		int sphereHitIndex;
		if (RaySphereIntersection(spherePoint, light.pos, intersection, sphereHitIndex, ignoreIndex))
		{
			if ((light.pos - intersection).Length() < lightDist)
			{
				return true;
			}
		}

		return false;
	}

	void RenderPixelsInterlace(int id, int offset, int threadCount)
	{
		for (int i = offset; i < WINDOW_WIDTH * WINDOW_HEIGHT; i += threadCount)
		{
			// dont draw pixels off screen
			if (i >= WINDOW_WIDTH * WINDOW_HEIGHT)
			{
				break;
			}
			
			// offset to pixel x, y
			int x = i % WINDOW_WIDTH;
			int y = i / WINDOW_WIDTH;

			float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
			// set up two points to define a single ray
			Vec3 p0 = CAMERA.pos;
			Vec3 p1 = Vec3(
				(x - WINDOW_WIDTH / 2) / (float)WINDOW_WIDTH + CAMERA.pos.x + (CAMERA.focalLength * sin(CAMERA.rot.y) * cos(CAMERA.rot.x)),
				((y - WINDOW_HEIGHT / 2) / (float)WINDOW_HEIGHT / aspectRatio) * -1.0f + CAMERA.pos.y + (CAMERA.focalLength * sin(CAMERA.rot.x)),
				CAMERA.pos.z + (CAMERA.focalLength * cos(CAMERA.rot.y) * cos(CAMERA.rot.x))
			);

			// find the closest sphere that the ray intersects
			Vec3 intersectionPos(0.0f, 0.0f, 0.0f);
			int sphereHitIndex = -1;
			if (RaySphereIntersection(p0, p1, intersectionPos, sphereHitIndex))
			{
				if (intersectionPos.z >= CAMERA.pos.z)
				{

					// calculate the normal of the sphere at the intersection point
					Vec3 normal(
						(intersectionPos.x - SPHERE_LIST[sphereHitIndex].pos.x) / SPHERE_LIST[sphereHitIndex].rad,
						(intersectionPos.y - SPHERE_LIST[sphereHitIndex].pos.y) / SPHERE_LIST[sphereHitIndex].rad,
						(intersectionPos.z - SPHERE_LIST[sphereHitIndex].pos.z) / SPHERE_LIST[sphereHitIndex].rad
					);

					// colour to be displayed
					Colour newCol(0, 0, 0);

					// sphere material
					Material mat = SPHERE_LIST[sphereHitIndex].mat;

					for (int k = 0; k < LIGHT_COUNT; k++)
					{
						// calculate the light direction
						Vec3 lightDir = (LIGHT_LIST[k].pos - intersectionPos).Normalise();

						// how much light should be reflected
						float distanceToLight = (LIGHT_LIST[k].pos - intersectionPos).Length();
						float lightFct = cos(normal.AngleBetween(lightDir)) * LIGHT_LIST[k].i * (1.0f / (distanceToLight * distanceToLight));

						// calculate the colour of the pixel
						Colour col(
							(mat.diff * lightFct + mat.amb) * mat.col.r,
							(mat.diff * lightFct + mat.amb) * mat.col.g,
							(mat.diff * lightFct + mat.amb) * mat.col.b
						);

						// set the pixel colour
						int originalSphereHitIndex = sphereHitIndex;
						if (SpherePointInShadow(intersectionPos, LIGHT_LIST[k], originalSphereHitIndex))
						{
							// if the point is in shadow, make it darker
							float shadowFct = 0.5f;
							newCol = newCol + (mat.col * mat.diff * (lightFct / LIGHT_LIST[k].i) * shadowFct);
						}
						else
						{
							// if the point is not in shadow, draw it normally, using the phong highlight model

							// calculate the reflection vector
							Vec3 CAMERADir = (CAMERA.pos - intersectionPos).Normalise();
							Vec3 halfway = (lightDir + CAMERADir).Normalise();

							// highlight factor
							float hf = pow((normal.Dot(halfway)), mat.refl);

							// add the diffuse and specular components
							newCol = newCol + (LIGHT_LIST[k].col * mat.col  * mat.diff * lightFct) + (LIGHT_LIST[k].col * mat.spec * hf);
						}
					}

					// add ambient component and clamp the colour
					newCol = newCol + (mat.col * mat.amb);

					// GAMMA correction
					newCol = Colour(
						pow(newCol.r / 255.0f, 1.0f / GAMMA) * 255.0f,
						pow(newCol.g / 255.0f, 1.0f / GAMMA) * 255.0f,
						pow(newCol.b / 255.0f, 1.0f / GAMMA) * 255.0f
					);

					// clamp colour
					newCol.Clamp();

					// draw the pixel
					DrawPixel(x, y, newCol);
				}
				else
				{
					// intersection is behind CAMERA, draw background
					Colour backgroundCol = TOP_BG_COL.Lerp(BOT_BG_COL, (float)y / WINDOW_HEIGHT);
					DrawPixel(x, y, backgroundCol);
				}
			}
			else
			{
				// no intersection, draw background
				Colour backgroundCol = TOP_BG_COL.Lerp(BOT_BG_COL, (float)y / WINDOW_HEIGHT);
				DrawPixel(x, y, backgroundCol);
			}
		}

		THREAD_STATUS[offset] = 1;
	}
}