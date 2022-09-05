#include <math.h>
#include <SFML/Graphics.hpp>

#include "colour.cpp"
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

bool RaySphereIntersection(Vec3 p0, Vec3 p1, Sphere sphereList[], int sphereCount, Vec3 &intersection, int &sphereHitIndex)
{
	float closestT = 1000000.0f;
	for (int i = 0; i < sphereCount; i++)
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

	if (closestT < 1000000.0f)
	{
		return true;
	}
	return false;
}

int main()
{
	const int windowWidth = 600;
	const int windowHeight = 600;
	const float aspectRatio = (float)windowHeight / (float)windowWidth;
	sf::Uint8 pixels[4 * windowWidth * windowHeight];

	Colour backgroundCol(153, 195, 255);

	sf::Image image;
	image.create(windowWidth, windowHeight, pixels);

	sf::Texture texture;
	texture.loadFromImage(image);

	sf::Sprite sprite;
	sprite.setPosition(0, 0);
	sprite.setTexture(texture);

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Window", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

	// list of all spheres in the scene
	Sphere sphereList[] = 
	{
		Sphere(Vec3(-0.8, -0.8, 5.0), 1.0, Colour(255, 128, 64)), // orange
		Sphere(Vec3(-1.8, 1.8, 10.0), 1.0, Colour(64, 255, 128)), // green
		Sphere(Vec3(1.0, 0.5, 5.25), 1.0, Colour(128, 64, 255)), // blue
	};

	// how many spheres
	int sphereCount = sizeof(sphereList) / sizeof(Sphere);

	// light
	Light light(-10.0, -7.0, 4.0, 1.0);

	//camera 
	Vec3 camera(0.0, 0.0, -5.0);


	sf::Clock clock;
	float lastTime = 0;
	while (window.isOpen())
	{
		// window event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// pixel to be tested
		Vec3 pixel(0.0, 0.0, 0.0);

		for (int i = 0; i < windowHeight; i++)
		{
			for (int j = 0; j < windowWidth; j++)
			{
				// pixel being tested (from -1 to 1 screen space)
				pixel.x = (2 * j) / (float)windowWidth - 1;
				pixel.y = ((2 * i) / (float)windowHeight - 1) * aspectRatio;

				Vec3 intersectionPoint;
				int sphereHitIndex;
				if (RaySphereIntersection(camera, pixel, sphereList, sphereCount, intersectionPoint, sphereHitIndex))
				{
					// calculate sphere surface normal (x, y, z from 0 to 1)
					Vec3 normal(
						(intersectionPoint.x - sphereList[sphereHitIndex].x) / sphereList[sphereHitIndex].rad,
						(intersectionPoint.y - sphereList[sphereHitIndex].y) / sphereList[sphereHitIndex].rad,
						(intersectionPoint.z - sphereList[sphereHitIndex].z) / sphereList[sphereHitIndex].rad
					);

					// unit vector pointing from intersection to light
					Vec3 lightDir = (light.pos - intersectionPoint).Normalise();

					// calculate diffuse lighting
					float factor = cos(normal.AngleBetween(lightDir));
					float kd = 0.8;
					float ka = 0.2;

					Colour col(
						(kd * factor + ka) * sphereList[sphereHitIndex].col.r,
						(kd * factor + ka) * sphereList[sphereHitIndex].col.g,
						(kd * factor + ka) * sphereList[sphereHitIndex].col.b
					);
					
					DrawPixel(j, i, pixels, windowWidth, windowHeight, col);
				}
				else
				{
					// pixel is in background
					if (RaySphereIntersection(pixel, light.pos, sphereList, sphereCount, intersectionPoint, sphereHitIndex))
					{
						DrawPixel(j, i, pixels, windowWidth, windowHeight, backgroundCol / 2);
					}
					else
					{
						DrawPixel(j, i, pixels, windowWidth, windowHeight, backgroundCol);
					}
				}
			}
		}

		window.clear();

		image.create(windowWidth, windowHeight, pixels);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		window.draw(sprite);

		window.display();

		window.setTitle("FPS: " + std::to_string((int)(1.f / (clock.restart().asSeconds() - lastTime))));
		float currentTime = clock.restart().asSeconds();
		lastTime = currentTime;
		
	}
	return 0;
}