#include "colour.cpp"
#include "light.cpp"
#include "sphere.cpp"
#include "utility.cpp"
#include "vec3.cpp"

int main()
{
	const int windowWidth = 600;
	const int windowHeight = 600;
	const float aspectRatio = (float)windowHeight / (float)windowWidth;
	sf::Uint8 pixels[4 * windowWidth * windowHeight];

	Colour topBackgroundCol(173, 200, 255);
	Colour bottomBackgroundCol(107, 171, 255);

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
		Sphere(Vec3(-0.8, -0.8, 9.0), 1.0, Colour(255, 128, 64)), // orange
		Sphere(Vec3(-0.8, 0.2, 7.0), 1.0, Colour(64, 255, 128)), // green
		Sphere(Vec3(1.2, 0.5, 7.75), 1.0, Colour(128, 64, 255)), // purple
		Sphere(Vec3(0.0, 10.0, 30.0), 1.0, Colour(255, 255, 255)), // white
	};

	int sphereCount = sizeof(sphereList) / sizeof(Sphere);

	// light
	Light light(0, 5, 2, 1.0);

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

		// keyboard input
		float speed = 0.1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            camera.x -= speed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            camera.x += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            camera.y -= speed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            camera.y += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            camera.z -= speed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            camera.z += speed;
		printf("camera: %f, %f, %f\n", camera.x, camera.y, camera.z);

		// render calculations
		for (int i = 0; i < windowHeight; i++)
		{
			for (int j = 0; j < windowWidth; j++)
			{
				// set up two points to define a ray
				Vec3 p0 = camera;
				Vec3 p1 = Vec3(
					(j - windowWidth / 2) / (float)windowWidth + camera.x,
					(i - windowHeight / 2) / (float)windowWidth * aspectRatio + camera.y,
					camera.z + 1.0
				);

				// find the closest sphere that the ray intersects
				Vec3 intersectionPos;
				int sphereHitIndex = -1;
				if (RaySphereIntersection(p0, p1, sphereList, sphereCount, intersectionPos, sphereHitIndex))
				{
					// calculate the normal of the sphere at the intersection point
					Vec3 normal(
						(intersectionPos.x - sphereList[sphereHitIndex].x) / sphereList[sphereHitIndex].rad,
						(intersectionPos.y - sphereList[sphereHitIndex].y) / sphereList[sphereHitIndex].rad,
						(intersectionPos.z - sphereList[sphereHitIndex].z) / sphereList[sphereHitIndex].rad
					);

					// calculate the light direction
					Vec3 lightDir = (light.pos - intersectionPos).Normalise();

					// how much light should be reflected
					float factor = cos(normal.AngleBetween(lightDir));

					// material properties
					float kd = 0.8; // diffuse
					float ka = 0.2; // ambient

					// calculate the colour of the pixel
					Colour col(
						(kd * factor + ka) * sphereList[sphereHitIndex].col.r,
						(kd * factor + ka) * sphereList[sphereHitIndex].col.g,
						(kd * factor + ka) * sphereList[sphereHitIndex].col.b
					);

					// set the pixel colour
					//DrawPixel(j, i, pixels, windowWidth, windowHeight, col);

					int originalSphereHitIndex = sphereHitIndex;
					if (SpherePointInShadow(intersectionPos, light, sphereList, sphereCount, originalSphereHitIndex))
					{
						DrawPixel(j, i, pixels, windowWidth, windowHeight, (sphereList[sphereHitIndex].col * ka) + (sphereList[sphereHitIndex].col * kd * factor * 0.2));
					}
					else
					{
						DrawPixel(j, i, pixels, windowWidth, windowHeight, col);	
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
		window.setTitle("FPS: " + std::to_string((int)(1.f / (clock.restart().asSeconds() - lastTime))));
		float currentTime = clock.restart().asSeconds();
		lastTime = currentTime;
		
	}
	return 0;
}