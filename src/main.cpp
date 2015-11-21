#include <SFML/Graphics.hpp>
#include "ResourcePath.h"
#include <iostream>
#include "ResourcePath.h"
#include "Overlap.h"
#include <cstdlib>
#include <time.h>
#include <iomanip>
#include <SFML/System.hpp>
#include <SFML/Graphics/Text.hpp> 
#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>
#include <sstream>

using namespace std;

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

void update(sf::RectangleShape floor[], int floorWidth, int floorHeightPosition, sf::Sprite& rrSprite, double &pos, bool &jumpStatus, 
	bool& isoverlap, std::stringstream &ss, sf::Clock &clock, sf::Text &text2, sf::Sprite& boulderSprite, int& eaten, int& boulderx)
{
	//game timer
	if (!isoverlap)
	{
		sf::Time time1 = clock.getElapsedTime();
		ss.str(std::string());
		ss << setprecision(2) << fixed << static_cast<double>(time1.asSeconds());
		text2.setString(ss.str().c_str());
	}

	//flooring
	int position = 15;

	floor[0].move(-position, 0);
	floor[1].move(-position, 0);

	if (floor[0].getPosition().x <= -floorWidth)
	{
		floor[0].setPosition(0, floorHeightPosition);
		floor[1].setPosition(floorWidth, floorHeightPosition);
	}

	//roadrunner starts here

     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && rrSprite.getPosition().y == 510)
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

	//boulder movement
	boulderSprite.move(-10, 0);
	boulderx -= 10;

	if (boulderx <= -100)
	{
		boulderSprite.setPosition(boulderx = 1000, 480);
	}

	if (overlap(rrSprite, boulderSprite))
	{
		eaten++;
		isoverlap = true;
		//	chari.setScale(0, 0);
	}
}





void draw(sf::RenderWindow& window, sf::RectangleShape floor[], sf::Sprite& rrSprite, 
	sf::Sprite& boulderSprite, sf::Text &text, sf::Text &text2, bool isoverlap)
{
	window.clear();

	if (isoverlap)
	{
		window.draw(text);
		window.draw(text2);
	}
	else
	{
		window.draw(floor[0]);
		window.draw(floor[1]);
		window.draw(boulderSprite);
		window.draw(rrSprite);
		window.draw(text2);

	}

	window.display();
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 800), "RoadRunner");
	window.setVerticalSyncEnabled(true);

	// Time
	sf::Clock clock;
	sf::Time time1 = clock.getElapsedTime();
	sf::Time total;

	// Display time
	std::stringstream ss;
	ss.str(std::string());
	ss << time1.asSeconds();
	sf::Text text2;
	text2.setString(ss.str().c_str());

	// Load and set game time font
	sf::Font font2;
	text2.setFont(font2);
	font2.loadFromFile(resourcePath() + "assets/Pacifico.ttf");
	text2.setCharacterSize(100);
	text2.setStyle(sf::Text::Bold);
	text2.setColor(sf::Color::Blue);
	text2.setPosition(0, 0);

	// Load and set game over font
	sf::Font font;
	font.loadFromFile(resourcePath() + "assets/Pacifico.ttf");
	sf::Text text("GAME OVER", font);
	text.setFont(font);

	text.setCharacterSize(100);
	text.setStyle(sf::Text::Bold);
	text.setColor(sf::Color::Blue);
	text.setPosition(100, 100);

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

	// boulder variables
	sf::Texture boulderTexture;
	boulderTexture.loadFromFile(resourcePath() + "assets/boulder.jpeg");
	sf::Sprite boulderSprite(boulderTexture);
	boulderSprite.setScale(0.7, 0.7);


	int boulderx = 1000;
	int bouldery = window.getSize().y + blockSize;

	boulderSprite.setPosition(boulderx, bouldery);

	// Overlap variables
	int eaten = 0;
	bool isoverlap = false;

	while (window.isOpen())
	{
		handleEvent(window);
		update(floor, floorWidth, floorHeightPosition, rrSprite, pos, jumpStatus, isoverlap, ss, clock, text2, boulderSprite, eaten, boulderx);
		draw(window, floor, rrSprite, boulderSprite, text, text2, isoverlap);
	}

	return 0;
}
