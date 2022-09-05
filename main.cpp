#include <math.h>
#include <SFML/Graphics.hpp>

#include "light.cpp"
#include "sphere.cpp"
#include "vec3.cpp"

void DrawPixel(int x, int y, sf::Uint8 *pixels, int windowWidth, int windowHeight, unsigned char r, unsigned char g, unsigned char b)
{
	pixels[(y * windowWidth + x) * 4 + 0] = (int)r;
	pixels[(y * windowWidth + x) * 4 + 1] = (int)g;
	pixels[(y * windowWidth + x) * 4 + 2] = (int)b;
	pixels[(y * windowWidth + x) * 4 + 3] = 255;
}

bool RaySphereIntersection(Vec3 p0, Vec3 p1, Sphere sphere, Vec3 &intersection)
{
	Vec3 d = p1 - p0;
	float a = d.x * d.x + d.y * d.y + d.z * d.z;
	float b = 2 * d.x * (p0.x - sphere.x) + 2 * d.y * (p0.y - sphere.y) + 2 * d.z * (p0.z - sphere.z);
	float c = sphere.x * sphere.x + sphere.y * sphere.y + sphere.z * sphere.z + p0.x * p0.x + p0.y * p0.y + p0.z * p0.z + -2 * (sphere.x * p0.x + sphere.y * p0.y + sphere.z * p0.z) - sphere.r * sphere.r;

	float discriminant = b * b - 4 * a * c;

	if (discriminant > 0)
	{
		float t = (-b - sqrt(discriminant)) / (2 * a);
		intersection = p0 + d * t;
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

	sf::Image image;
	image.create(windowWidth, windowHeight, pixels);

	sf::Texture texture;
	texture.loadFromImage(image);

	sf::Sprite sprite;
	sprite.setPosition(0, 0);
	sprite.setTexture(texture);

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Window", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

	// sphere
	Sphere sphere(0.0, 0.0, 6.0, 1.0);

	// light
	Light light(-2.0, -3.0, 2.0, 1.0);

	//camera 
	Vec3 camera(0.0, 0.0, 0.0);


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
		Vec3 pixel(0.0, 0.0, 5.0);

		for (int i = 0; i < windowHeight; i++)
		{
			for (int j = 0; j < windowWidth; j++)
			{
				// pixel being tested (from -1 to 1 screen space)
				pixel.x = (2 * j) / (float)windowWidth - 1;
				pixel.y = ((2 * i) / (float)windowHeight - 1) * aspectRatio;

				Vec3 intersectionPoint;
				if (RaySphereIntersection(camera, pixel, sphere, intersectionPoint))
				{
					// calculate sphere surface normal (x, y, z from 0 to 1)
					Vec3 normal(
						(intersectionPoint.x - sphere.x) / sphere.r,
						(intersectionPoint.y - sphere.y) / sphere.r,
						(intersectionPoint.z - sphere.z) / sphere.r
					);

					// unit vector pointing from intersection to light
					Vec3 lightDir = (light.pos - intersectionPoint).Normalise();

					// calculate diffuse lighting
					float factor = cos(normal.AngleBetween(lightDir));
					float kd = 0.8;
					float ka = 0.2;
					float col = (kd * factor + ka) * 255;
					
					DrawPixel(j, i, pixels, windowWidth, windowHeight, col, col, col);
				}
				else
				{
					DrawPixel(j, i, pixels, windowWidth, windowHeight, 0, 0, 0);
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