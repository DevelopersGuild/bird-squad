#include <SFML/Graphics.hpp>
#include "ResourcePath.h"

void reset(sf::Sprite&)
{

}

void handleCollision(sf::Sprite&)
{

}

void handleEvent(sf::RenderWindow& window)
{
     sf::Event event;
     while (window.pollEvent(event))
     {
          if (event.type == sf::Event::Closed)
               window.close();
     }
}

void update(sf::RectangleShape floor[], int floorWidth, int floorHeightPosition)
{
     int position = 15;

     floor[0].move(-position, 0);
     floor[1].move(-position, 0);

     if (floor[0].getPosition().x <= -floorWidth)
     {
          floor[0].setPosition(0, floorHeightPosition);
          floor[1].setPosition(floorWidth, floorHeightPosition);
     }
}

void draw(sf::RenderWindow& window, sf::RectangleShape floor[])
{
     window.clear();
     window.draw(floor[0]);
     window.draw(floor[1]);
     window.display();
}

int main()
{
     sf::RenderWindow window(sf::VideoMode(1000, 800), "RoadRunner");
     window.setVerticalSyncEnabled(true);

     int floorWidth = window.getSize().x;

     int blockSize = 200;
     int floorHeightPosition = window.getSize().y - blockSize;

     sf::Texture floorTexture;
     floorTexture.loadFromFile(resourcePath() + "assets/yellow_floor.jpg");
 
     sf::RectangleShape floor[2];

     for (int x = 0; x < 2; x++)
     {
          floor[x].setSize(sf::Vector2f(floorWidth, blockSize));
          floor[x].setTexture(&floorTexture);
          floor[x].setPosition(floorWidth * x, floorHeightPosition);
     }

	while (window.isOpen())
	{
          handleEvent(window);
          update(floor, floorWidth, floorHeightPosition);
          draw(window, floor);
	}

	return 0;
}
