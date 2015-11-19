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

void update(sf::RectangleShape floor[], int floorWidth, int floorHeightPosition, sf::Sprite& rrSprite, double &pos)
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (pos == 510)) //pos == 510 is to prevent the player from spamming the up arrow key or hold so that roadrunner can stay in the air
	{

		rrSprite.setPosition(25, pos);
		pos -= 5;
		while (pos != 310)
		{
			rrSprite.setPosition(25, pos);
			pos -= 5;
		}
	}

	else if (pos != 510) //This else if statement resets the roadrunners position in increments of pos += 
	{
		rrSprite.setPosition(25, pos);
		pos += 5; //Changing 5 will change how fast the roadrunner falls down after the jump higher values means falls down faster and vice versa
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
		update(floor, floorWidth, floorHeightPosition, rrSprite, pos);
		draw(window, floor, rrSprite);
	}

	return 0;
}
