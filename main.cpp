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
		Sphere(Vec3(-0.8f, -0.8f,  9.0f), 1.0f, Material(Colour(255, 128,  64), 0.3f, 1.0f, 1500.0f, 0.08f)), // orange
		Sphere(Vec3(-0.8f,  0.2f,  7.0f), 1.0f, Material(Colour( 64, 255, 128), 0.4f, 0.5f,  200.0f, 0.08f)), // green
		Sphere(Vec3(1.2f,   0.5f,  7.7f), 1.0f, Material(Colour(128,  64, 255), 0.6f, 0.2f,   30.0f, 0.08f)), // purple
		Sphere(Vec3(1.2f,  -1.5f, 10.0f), 1.0f, Material(Colour(255, 255, 255), 0.7f, 0.4f,   20.0f, 0.08f)), // white
	};

	// list of all lights in the scene
	Light lightList[] = 
	{
		Light(Vec3(-5.0f,  -5.0f, 5.0f), Colour(255, 128, 128), 50.0f), // red
		Light(Vec3( 0.0f, -10.0f, 4.0f), Colour(128, 255, 128), 80.0f), // green
		Light(Vec3( 3.0f,  -2.0f, 4.0f), Colour(128, 128, 255), 30.0f), // blue
		Light(Vec3(-3.0f,  -3.0f, 9.0f), Colour(255, 255, 255),  8.0f)  // white
	};

	// sphere and light count
	const int sphereCount = sizeof(sphereList) / sizeof(Sphere);
	const int lightCount = sizeof(lightList) / sizeof(Light);

	//camera 
	Camera camera(Vec3(0.0f, 0.0f, 0.0f), 90.0f);

	float gamma = 2.2f;

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

					// colour to be displayed
					Colour newCol(0, 0, 0);

					// sphere material
					Material mat = sphereList[sphereHitIndex].mat;

					for (int k = 0; k < lightCount; k++)
					{
						// calculate the light direction
						Vec3 lightDir = (lightList[k].pos - intersectionPos).Normalise();

						// how much light should be reflected
						float distanceToLight = (lightList[k].pos - intersectionPos).Length();
						float lightFct = cos(normal.AngleBetween(lightDir)) * lightList[k].i * (1.0f / (distanceToLight * distanceToLight));

						// calculate the colour of the pixel
						Colour col(
							(mat.diff * lightFct + mat.amb) * mat.col.r,
							(mat.diff * lightFct + mat.amb) * mat.col.g,
							(mat.diff * lightFct + mat.amb) * mat.col.b
						);

						// set the pixel colour
						int originalSphereHitIndex = sphereHitIndex;
						if (SpherePointInShadow(intersectionPos, lightList[k], sphereList, sphereCount, originalSphereHitIndex))
						{
							// if the point is in shadow, make it darker
							float shadowFct = 0.5f;
							newCol = newCol + (mat.col * mat.diff * (lightFct / lightList[k].i) * shadowFct);
						}
						else
						{
							// if the point is not in shadow, draw it normally, using the phong highlight model

							// calculate the reflection vector
							Vec3 cameraDir = (camera.pos - intersectionPos).Normalise();
							Vec3 halfway = (lightDir + cameraDir).Normalise();

							// highlight factor
							float hf = pow((normal.Dot(halfway)), mat.refl);

							// add the diffuse and specular components
							newCol = newCol + (lightList[k].col * mat.col  * mat.diff * lightFct) + (lightList[k].col * mat.spec * hf);
						}
					}

					// add ambient component and clamp the colour
					newCol = newCol + (mat.col * mat.amb);

					// gamma correction
					newCol = Colour(
						pow(newCol.r / 255.0f, 1.0f / gamma) * 255.0f,
						pow(newCol.g / 255.0f, 1.0f / gamma) * 255.0f,
						pow(newCol.b / 255.0f, 1.0f / gamma) * 255.0f
					);

					// clamp colour
					newCol.Clamp();

					// draw the pixel
					DrawPixel(j, i, pixels, windowWidth, windowHeight, newCol);
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