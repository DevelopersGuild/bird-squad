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

void update(sf::RectangleShape floor[], int floorWidth, int floorHeightPosition, sf::Sprite& rrSprite, double &pos, bool &jumpStatus)
{

	int position = 15;

	floor[0].move(-position, 0);
	floor[1].move(-position, 0);

	if (floor[0].getPosition().x <= -floorWidth)
	{
		floor[0].setPosition(0, floorHeightPosition);
		floor[1].setPosition(floorWidth, floorHeightPosition);
	}

	//roadrunner starts here

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		jumpStatus = 1;
	}

	if (jumpStatus == 1 && (rrSprite.getPosition().y <= 510))
	{
		rrSprite.move(0, -10);
	}

	if (rrSprite.getPosition().y == 100)
	{
		jumpStatus = 0;
	}

	if (jumpStatus == 0 && (rrSprite.getPosition().y < 510))
	{
		rrSprite.move(0, 10);
	}

}





void draw(sf::RenderWindow& window, sf::RectangleShape floor[], sf::Sprite& rrSprite)
{
	window.clear();
	window.draw(floor[0]);
	window.draw(floor[1]);
	window.draw(rrSprite);
	window.display();
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 800), "RoadRunner");
	window.setVerticalSyncEnabled(true);

	//roadrunner variables
	sf::Texture rrTexture;
	rrTexture.loadFromFile(resourcePath() + "assets/roadrunner.png");
	sf::Sprite rrSprite(rrTexture);
	double pos = 510;
	bool jumpStatus = 0;
	rrSprite.setPosition(25, pos);

	//floor variables
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
		update(floor, floorWidth, floorHeightPosition, rrSprite, pos, jumpStatus);
		draw(window, floor, rrSprite);
	}

	return 0;
}
