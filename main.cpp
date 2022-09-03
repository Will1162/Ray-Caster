#include <SFML/Graphics.hpp>

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
    const int windowHeight = 600;
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

    float sphereX = 0.0;
    float sphereY = 0.0;
    float sphereZ = 20.0;


    sf::Clock clock;
    float lastTime = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // float mouseX = sf::Mouse::getPosition(window).x / (float)windowWidth;
        // float mouseY = sf::Mouse::getPosition(window).y / (float)windowHeight;

        float speed = 0.1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            sphereX -= speed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            sphereX += speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            sphereY -= speed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            sphereY += speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            sphereZ -= speed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            sphereZ += speed;
        }


        for (int i = 0; i < windowHeight; i++)
        {
            for (int j = 0; j < windowWidth; j++)
            {
                // sphere
                float cx = sphereX;
                float cy = sphereY;
                float cz = sphereZ;
                float r  = 1.0;
                
                // float cx = 2 * mouseX - 1;
                // float cy = 2 * mouseY - 1;
                // float cz = 20.0;
                // float r  = 1.0;

                // camera
                float x0 = 0.0;
                float y0 = 0.0;
                float z0 = 0.0;

                // pixel/ray being tested
                float x1 = (2 * j) / (float)windowWidth - 1;
                float y1 = (2 * i) / (float)windowHeight - 1;
                float z1 = 5.0;

                // ray vector direction
                float dx = x1 - x0;
                float dy = y1 - y0;
                float dz = z1 - z0;

                // ray-sphere intersection
                float a = dx * dx + dy * dy + dz * dz;
                float b = 2 * (dx * (x0 - cx) + dy * (y0 - cy) + dz * (z0 - cz));
                float c = cx * cx + cy * cy + cz * cz + x0 * x0 + y0 * y0 + z0 * z0 - 2 * (cx * x0 + cy * y0 + cz * z0) - r * r;

                // discriminant
                float d = b * b - 4 * a * c;

                // draw pixels if ray intersects sphere
                if (d > 0)
                {
                    DrawPixel(j, i, pixels, windowWidth, windowHeight, 255, 255, 255);
                }
                else
                {
                    DrawPixel(j, i, pixels, windowWidth, windowHeight, 0, 0, 0);
                }
            }
        }

        image.create(windowWidth, windowHeight, pixels);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        window.draw(sprite);


        window.display();

        window.setTitle("FPS: " + std::to_string((int)(1.f / (clock.restart().asSeconds() - lastTime))));

        float currentTime = clock.restart().asSeconds();
        float fps = 1.f / (currentTime - lastTime);
        lastTime = currentTime;
        
    }
    return 0;
}