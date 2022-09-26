#include <math.h>
#include <thread>

#include <SFML/Graphics.hpp>

#include "camera.cpp"
#include "colour.cpp"
#include "light.cpp"
#include "material.cpp"
#include "sphere.cpp"
#include "utility.cpp"
#include "vec3.cpp"

// threading global variables
static int pixelsRendered = 0;
static int availableThreads = 16;
static int totalThreads = 16;

// window global variables
static const int WINDOW_WIDTH = 512;
static const int WINDOW_HEIGHT = 512;
static const int BLOCK_SIZE_X = WINDOW_WIDTH / sqrt(totalThreads);
static const int BLOCK_SIZE_Y = WINDOW_HEIGHT / sqrt(totalThreads);
static const float GAMMA = 2.2f;

// pixel buffer
static sf::Uint8 PIXELS[4 * WINDOW_WIDTH * WINDOW_HEIGHT];

// background colours
static Colour TOP_BG_COL(173, 200, 255);
static Colour BOT_BG_COL(107, 171, 255);

// list of all spheres in the scene
static  Sphere SPHERE_LIST[] = 
{
	Sphere(Vec3(-0.8f,  0.8f,  9.0f), 1.0f, Material(Colour(255, 128,  64), 0.3f, 1.0f, 1500.0f, 0.08f)), // orange
	Sphere(Vec3(-0.8f, -0.2f,  7.0f), 1.0f, Material(Colour( 64, 255, 128), 0.4f, 0.5f,  200.0f, 0.08f)), // green
	Sphere(Vec3(1.2f,  -0.5f,  7.7f), 1.0f, Material(Colour(128,  64, 255), 0.6f, 0.2f,   30.0f, 0.08f)), // purple
	Sphere(Vec3(1.2f,   1.5f, 10.0f), 1.0f, Material(Colour(255, 255, 255), 0.7f, 0.4f,   20.0f, 0.08f)), // white
};

// list of all lights in the scene
static Light LIGHT_LIST[] = 
{
	Light(Vec3(-5.0f,  5.0f, 5.0f), Colour(255, 128, 128), 50.0f), // red
	Light(Vec3( 0.0f, 10.0f, 4.0f), Colour(128, 255, 128), 80.0f), // green
	Light(Vec3( 3.0f,  2.0f, 4.0f), Colour(128, 128, 255), 30.0f), // blue
	Light(Vec3(-3.0f,  3.0f, 9.0f), Colour(255, 255, 255),  8.0f)  // white
};

// sphere and light count
static const int SPHRE_COUNT = sizeof(SPHERE_LIST) / sizeof(Sphere);
static const int LIGHT_COUNT = sizeof(LIGHT_LIST) / sizeof(Light);

//camera 
static Camera CAMERA(Vec3(0.0f, 0.0f, 0.0f), 90.0f);

// render block of pixels at x, y, with width and height of BLOCK_SIZE_X and BLOCK_SIZE_Y
void RenderPixelBlock(int x, int y)
{
	for (int i = 0; i < BLOCK_SIZE_X; i++)
	{
		for (int j = 0; j < BLOCK_SIZE_Y; j++)
		{
			float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
			// set up two points to define a single ray
			Vec3 p0 = CAMERA.pos;
			Vec3 p1 = Vec3(
				(x + i - WINDOW_WIDTH / 2) / (float)WINDOW_WIDTH + CAMERA.pos.x,
				((y + j - WINDOW_HEIGHT / 2) / (float)WINDOW_HEIGHT / aspectRatio) * -1.0f + CAMERA.pos.y,
				CAMERA.pos.z + CAMERA.focalLength
			);

			// find the closest sphere that the ray intersects
			Vec3 intersectionPos(0.0f, 0.0f, 0.0f);
			int sphereHitIndex = -1;
			if (RaySphereIntersection(p0, p1, SPHERE_LIST, SPHRE_COUNT, intersectionPos, sphereHitIndex))
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
						if (SpherePointInShadow(intersectionPos, LIGHT_LIST[k], SPHERE_LIST, SPHRE_COUNT, originalSphereHitIndex))
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
					DrawPixel(x + i, y + j, PIXELS, WINDOW_WIDTH, WINDOW_HEIGHT, newCol);
				}
				else
				{
					// intersection is behind CAMERA, draw background
					Colour backgroundCol = TOP_BG_COL.Lerp(BOT_BG_COL, (float)(y + j) / WINDOW_HEIGHT);
					DrawPixel(x + i, y + j, PIXELS, WINDOW_WIDTH, WINDOW_HEIGHT, backgroundCol);
				}
			}
			else
			{
				// no intersection, draw background
				Colour backgroundCol = TOP_BG_COL.Lerp(BOT_BG_COL, (float)(y + j) / WINDOW_HEIGHT);
				DrawPixel(x + i, y + j, PIXELS, WINDOW_WIDTH, WINDOW_HEIGHT, backgroundCol);
			}
		}
	}

	availableThreads++;
	pixelsRendered++;
}

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
	window.setFramerateLimit(60);

	// available threads
	availableThreads = totalThreads;
	if (totalThreads <= 0)
		totalThreads = 1;

	printf("Block size: X: %i, Y: %i\n", BLOCK_SIZE_X, BLOCK_SIZE_Y);
	printf("Available threads: %d\n", totalThreads);

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
		float speed = 0.1f;
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

		// render calculations
		pixelsRendered = 0;

		// set up threads
		std::vector<std::thread> threads(totalThreads);
		int* availableThreadArray = (int*)malloc(sizeof(int) * totalThreads);
		for (int i = 0; i < totalThreads; i++)
		{
			availableThreadArray[i] = 1;
		}

		// start threads rendering blocks of pixels
		int x = 0;
		int y = 0;
		int count = 0;
		// while there are still pixels to render
		while (count < (WINDOW_WIDTH * WINDOW_HEIGHT) / (BLOCK_SIZE_X * BLOCK_SIZE_Y))
		{
			// re-join threads that have finished
			for (int i = 0; i < (int)threads.size(); i++)
			{
				if (threads[i].joinable())
				{
					threads[i].join();
					availableThreadArray[i] = 1;
				}
			}

			// start threads that are available
			for (int i = 0; i < (int)threads.size(); i++)
			{
				if (availableThreadArray[i] == 1)
				{
					// start thread and set it to unavailable
					threads[i] = std::thread(RenderPixelBlock, x, y);
					availableThreadArray[i] = 0;

					// move to next block
					x += BLOCK_SIZE_X;
					if (x >= WINDOW_WIDTH)
					{
						x = 0;
						y += BLOCK_SIZE_Y;
					}

					count++;
				}
			}
		}

		// wait for all threads to finish
		bool allThreadsDone = false;
		while (!allThreadsDone)
		{
			allThreadsDone = true;
			for (int i = 0; i < (int)threads.size(); i++)
			{
				if (threads[i].joinable())
				{
					threads[i].join();
					availableThreadArray[i] = 1;
				}
			}
			for (int i = 0; i < (int)threads.size(); i++)
			{
				if (availableThreadArray[i] == 0)
				{
					allThreadsDone = false;
					break;
				}
			}
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