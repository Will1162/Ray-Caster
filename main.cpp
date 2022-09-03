#include <random>
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


        for (int i = 0; i < windowHeight; i++)
        {
            for (int j = 0; j < windowWidth; j++)
            {
                DrawPixel(j, i, pixels, windowWidth, windowHeight, 0, 0, 0);
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