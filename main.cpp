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

int main()
{   
	const int windowWidth = 800;
	const int windowHeight = 450;
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
	Sphere sphere(0.0, 0.0, 10.0, 1.0);

	// light
	Light light(-2.0, -3.0, 15.0, 1.0);

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

				// ray vector
				Vec3 ray = pixel - camera;

				// ray-sphere intersection
				float a = ray.x * ray.x + ray.y * ray.y + ray.z * ray.z;
				float b = 2 * (ray.x * (camera.x - sphere.x) + ray.y * (camera.y - sphere.y) + ray.z * (camera.z - sphere.z));
				float c = sphere.x * sphere.x + sphere.y * sphere.y + sphere.z * sphere.z + camera.x * camera.x + camera.y * camera.y + camera.z * camera.z - 2 * (sphere.x * camera.x + sphere.y * camera.y + sphere.z * camera.z) - sphere.r * sphere.r;

				// discriminant
				float d = b * b - 4 * a * c;

				// get co-oridinates of intersection
				float t = (-b - sqrt(d)) / (2 * a);
				Vec3 point = Vec3(
					camera.x + t * ray.x,
					camera.y + t * ray.y,
					camera.z + t * ray.z
				);

				// calculate sphere surface normal (x, y, z from 0 to 1)
				Vec3 normal = (sphere.centre - point).normalise();
				normal = normal + 1.0;
				normal = normal * 0.5;

				// draw pixels if ray intersects sphere
				if (d > 0)
				{
					DrawPixel(j, i, pixels, windowWidth, windowHeight, normal.x * 255, normal.y * 255, normal.z * 255);
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