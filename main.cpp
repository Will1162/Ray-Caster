#include <math.h>

#include <SFML/Graphics.hpp>

#include "camera.cpp"
#include "colour.cpp"
#include "light.cpp"
#include "material.cpp"
#include "sphere.cpp"
#include "utility.cpp"
#include "vec3.cpp"

int main()
{
	// window properties
	const int windowWidth = 600;
	const int windowHeight = 600;
	const float aspectRatio = (float)windowWidth / (float)windowHeight;
	sf::Uint8 pixels[4 * windowWidth * windowHeight];

	// background colours
	Colour topBackgroundCol(173, 200, 255);
	Colour bottomBackgroundCol(107, 171, 255);

	// objects used to display pixels[]
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	image.create(windowWidth, windowHeight, pixels);
	texture.loadFromImage(image);
	sprite.setPosition(0.0f, 0.0f);
	sprite.setTexture(texture);

	// window creation
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Window", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

	// list of all spheres in the scene
	Sphere sphereList[] = 
	{
		Sphere(Vec3(-0.8f, -0.8f,  9.0f), 1.0f, Material(Colour(255, 128,  64), 0.3f, 1.0f, 1500.0f, 0.2f)), // orange
		Sphere(Vec3(-0.8f,  0.2f,  7.0f), 1.0f, Material(Colour(64,  255, 128), 0.4f, 0.5f, 200.0f,  0.2f)), // green
		Sphere(Vec3(1.2f,   0.5f,  7.7f), 1.0f, Material(Colour(128, 64,  255), 0.6f, 0.2f, 30.0f,   0.2f)), // purple
		Sphere(Vec3(0.0f,  10.0f, 30.0f), 1.0f, Material(Colour(255, 255, 255), 0.8f, 0.5f, 250.0f,  0.2f)), // white
	};

	const int sphereCount = sizeof(sphereList) / sizeof(Sphere);

	// light
	Light light(Vec3(-5.0f, -5.0f, 5.0f));

	//camera 
	Camera camera(Vec3(0.0f, 0.0f, 0.0f), 90.0f);

	// fps counter
	sf::Clock clock;
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
            camera.pos.x -= speed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            camera.pos.x += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            camera.pos.y -= speed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            camera.pos.y += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            camera.pos.z -= speed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            camera.pos.z += speed;

		// render calculations
		for (int i = 0; i < windowHeight; i++)
		{
			for (int j = 0; j < windowWidth; j++)
			{
				// set up two points to define a single ray
				Vec3 p0 = camera.pos;
				Vec3 p1 = Vec3(
					(j - windowWidth / 2) / (float)windowWidth + camera.pos.x,
					(i - windowHeight / 2) / (float)windowHeight / aspectRatio + camera.pos.y,
					camera.pos.z + camera.focalLength
				);

				// find the closest sphere that the ray intersects
				Vec3 intersectionPos(0.0f, 0.0f, 0.0f);
				int sphereHitIndex = -1;
				if (RaySphereIntersection(p0, p1, sphereList, sphereCount, intersectionPos, sphereHitIndex))
				{
					// calculate the normal of the sphere at the intersection point
					Vec3 normal(
						(intersectionPos.x - sphereList[sphereHitIndex].pos.x) / sphereList[sphereHitIndex].rad,
						(intersectionPos.y - sphereList[sphereHitIndex].pos.y) / sphereList[sphereHitIndex].rad,
						(intersectionPos.z - sphereList[sphereHitIndex].pos.z) / sphereList[sphereHitIndex].rad
					);

					// calculate the light direction
					Vec3 lightDir = (light.pos - intersectionPos).Normalise();

					// how much light should be reflected
					float lightFct = cos(normal.AngleBetween(lightDir));

					// calculate the colour of the pixel
					Material mat = sphereList[sphereHitIndex].mat;
					Colour col(
						(mat.diff * lightFct + mat.amb) * mat.col.r,
						(mat.diff * lightFct + mat.amb) * mat.col.g,
						(mat.diff * lightFct + mat.amb) * mat.col.b
					);

					// set the pixel colour
					int originalSphereHitIndex = sphereHitIndex;
					if (SpherePointInShadow(intersectionPos, light, sphereList, sphereCount, originalSphereHitIndex))
					{
						// if the point is in shadow, make it darker
						DrawPixel(j, i, pixels, windowWidth, windowHeight, (mat.col * mat.amb) + (mat.col * mat.diff * lightFct * 0.2f));
					}
					else
					{
						// if the point is not in shadow, draw it normally, also with phong highlights
						// calculate the reflection vector
						Vec3 cameraDir = (camera.pos - intersectionPos).Normalise();
						Vec3 halfway = (lightDir + cameraDir).Normalise();

						// highlight factor
						float hf = pow((normal.Dot(halfway)), mat.refl);

						// add the ambient, diffuse and specular components
						Colour newCol((mat.col * mat.amb) + (mat.col * mat.diff * lightFct) + (mat.spec * hf * 255));

						DrawPixel(j, i, pixels, windowWidth, windowHeight, newCol);
					}
				}
				else
				{
					// draw background
					Colour backgroundCol = topBackgroundCol.Lerp(bottomBackgroundCol, (float)i / windowHeight);
					DrawPixel(j, i, pixels, windowWidth, windowHeight, backgroundCol);
				}
			}
		}

		// render
		window.clear();
		image.create(windowWidth, windowHeight, pixels);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		window.draw(sprite);
		window.display();

		// fps counter
		window.setTitle("FPS: " + std::to_string((int)(1.0f / (clock.restart().asSeconds() - lastTime))));
		float currentTime = clock.restart().asSeconds();
		lastTime = currentTime;
		
	}
	return 0;
}