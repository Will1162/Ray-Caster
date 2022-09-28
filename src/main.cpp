#include <thread>
#include <math.h>

#include "../lib/ctpl_stl.hpp"

#include "camera.hpp"
#include "colour.hpp"
#include "light.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "utility.hpp"
#include "vec3.hpp"

// threading global variables
int pixelsRendered = 0;
int availableThreads = std::thread::hardware_concurrency();
int totalThreads = std::thread::hardware_concurrency();

// window global variables
const int WINDOW_WIDTH = 512;
const int WINDOW_HEIGHT = 512;
const float GAMMA = 2.2f;

// pixel buffer
sf::Uint8 PIXELS[4 * WINDOW_WIDTH * WINDOW_HEIGHT];

// background colours
WB_RT::Colour TOP_BG_COL(173, 200, 255);
WB_RT::Colour BOT_BG_COL(107, 171, 255);

// list of all spheres in the scene
WB_RT::Sphere SPHERE_LIST[] = 
{
	WB_RT::Sphere(WB_RT::Vec3(-0.8f,  0.8f,  9.0f), 1.0f, WB_RT::Material(WB_RT::Colour(255, 128,  64), 0.3f, 1.0f, 1500.0f, 0.08f)), // orange
	WB_RT::Sphere(WB_RT::Vec3(-0.8f, -0.2f,  7.0f), 1.0f, WB_RT::Material(WB_RT::Colour( 64, 255, 128), 0.4f, 0.5f,  200.0f, 0.08f)), // green
	WB_RT::Sphere(WB_RT::Vec3(1.2f,  -0.5f,  7.7f), 1.0f, WB_RT::Material(WB_RT::Colour(128,  64, 255), 0.6f, 0.2f,   30.0f, 0.08f)), // purple
	WB_RT::Sphere(WB_RT::Vec3(1.2f,   1.5f, 10.0f), 1.0f, WB_RT::Material(WB_RT::Colour(255, 255, 255), 0.7f, 0.4f,   20.0f, 0.08f)), // white
};

// list of all lights in the scene
WB_RT::Light LIGHT_LIST[] = 
{
	WB_RT::Light(WB_RT::Vec3(-5.0f,  5.0f, 5.0f), WB_RT::Colour(255, 128, 128), 50.0f), // red
	WB_RT::Light(WB_RT::Vec3( 0.0f, 10.0f, 4.0f), WB_RT::Colour(128, 255, 128), 80.0f), // green
	WB_RT::Light(WB_RT::Vec3( 3.0f,  2.0f, 4.0f), WB_RT::Colour(128, 128, 255), 30.0f), // blue
	WB_RT::Light(WB_RT::Vec3(-3.0f,  3.0f, 9.0f), WB_RT::Colour(255, 255, 255),  8.0f)  // white
};

// sphere and light count
const int SPHERE_COUNT = sizeof(SPHERE_LIST) / sizeof(WB_RT::Sphere);
const int LIGHT_COUNT = sizeof(LIGHT_LIST) / sizeof(WB_RT::Light);

//camera 
WB_RT::Camera CAMERA(WB_RT::Vec3(0.0f, 0.0f, 0.0f), WB_RT::Vec3(0.0f, 0.0f, 0.0f), 90.0f);

int main()
{
	// objects used to display PIXELS[]
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	image.create(WINDOW_WIDTH, WINDOW_HEIGHT, PIXELS);
	texture.loadFromImage(image);
	sprite.setPosition(0.0f, 0.0f);
	sprite.setTexture(texture);

	// window creation
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Window", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(100);

	// thread pool
	ctpl::thread_pool threadPool(availableThreads);

	// available threads
	availableThreads = totalThreads;
	if (totalThreads <= 0)
		totalThreads = 1;

	printf("System threads: %d\n", totalThreads + 1);

	// fps counter
	sf::Clock clock;
	float lastFpsList[5];
	float lastTime = 0.0f;

	while (window.isOpen())
	{
		// window event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// keyboard input
		float speed = 400.0f * lastTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            CAMERA.pos.x -= speed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            CAMERA.pos.x += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            CAMERA.pos.y -= speed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            CAMERA.pos.y += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            CAMERA.pos.z -= speed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            CAMERA.pos.z += speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			CAMERA.rot.x += speed * 0.25f;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			CAMERA.rot.x -= speed * 0.25f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            CAMERA.rot.y -= speed * 0.25f;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            CAMERA.rot.y += speed * 0.25f;

		// render calculations
		pixelsRendered = 0;

		int threadsRendered = 0;
		// while there are still pixels to render
		while (threadsRendered < totalThreads)
		{
			// push new render task to thread pool
			threadPool.push(WB_RT::RenderPixelsInterlace, threadsRendered, totalThreads);
			threadsRendered++;
		}

		// render
		window.clear();
		image.create(WINDOW_WIDTH, WINDOW_HEIGHT, PIXELS);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		window.draw(sprite);
		window.display();

		// fps counter
		float fps = 1.0f / (clock.restart().asSeconds() - lastTime);
		float avgFps = 0.0f;
		for (int i = 0; i < 4; i++)
		{
			lastFpsList[i] = lastFpsList[i + 1];
			avgFps += lastFpsList[i];
		}
		avgFps /= 5.0f;
		lastFpsList[4] = fps;

		window.setTitle("FPS: " + std::to_string((int)avgFps));
		float currentTime = clock.restart().asSeconds();
		lastTime = currentTime;
		
	}
	return 0;
}