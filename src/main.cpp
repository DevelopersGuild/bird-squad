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
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

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

void update(sf::RectangleShape floor[], sf::RectangleShape road[], int floorWidth, int floorHeightPosition, sf::Sprite& rrSprite, double &pos, bool &jumpStatus,
     bool& isoverlap, std::stringstream &ss, sf::Clock &clock, sf::Text &text2, sf::Sprite& boulderSprite, int& eaten, int& boulderx, sf::Music &music, 
     sf::Sound &sound, sf::SoundBuffer &squawk, bool &deathSound, sf::Sprite& sanicSprite, sf::Sprite& sanicPowerupSprite, bool& sanicPowerupStatus, int& globalSpeed, sf::Time& sanicTime,
     bool& powerupSpawnStatus, sf::Time& powerupSpawnTimer)
{
	//game timer
     if (!isoverlap)
	{
		sf::Time time1 = clock.getElapsedTime();
		ss.str(std::string());
		ss << setprecision(2) << fixed << static_cast<double>(time1.asSeconds());
		text2.setString(ss.str().c_str());
	}

     if (clock.getElapsedTime() >= sanicTime && sanicPowerupStatus == 1)
     {
          music.stop();
          music.openFromFile(resourcePath() + "assets/roadrunner_theme.wav");
          music.setVolume(100);
          music.play();
          sanicPowerupStatus = 0;
     }

     // How fast everything moves
     if (sanicPowerupStatus && (clock.getElapsedTime() <= sanicTime - sf::seconds(1)))         // slows down at the end of run
          globalSpeed = 50;
     else
          globalSpeed = 15;

     // Powerup spawner, checks every 10 seconds
     if (clock.getElapsedTime() >= powerupSpawnTimer)
     {
          powerupSpawnTimer += sf::seconds(10);                       //time to spawn for every consecutive powerup after the first
          powerupSpawnStatus = 1;
     }

     // Powerup spawn chance
     if (powerupSpawnStatus)
     {
          if (rand() % 100 + 1 <= 20)                                 // 20% chance to spawn
               sanicPowerupSprite.setPosition(floorWidth, 300);
          powerupSpawnStatus = 0;
     }

     // Sanicpowerup movement, how fast the icon moves
     if (sanicPowerupSprite.getPosition().x >= -floorWidth)
          sanicPowerupSprite.move(-15, 0);

     // Roadrunner and sanicpowerup collision
     if (overlap(rrSprite, sanicPowerupSprite) && sanicPowerupStatus == 0)
     {
          music.stop();
          music.openFromFile(resourcePath() + "assets/sanic_theme.wav");
          music.setVolume(15);
          music.play();
          sanicPowerupStatus = 1;
          sanicTime = clock.getElapsedTime() + sf::seconds(10);
     }

     //Infinite floor/road
     floor[0].move(-globalSpeed, 0);
     floor[1].move(-globalSpeed, 0);

     road[0].move(-globalSpeed, 0);
     road[1].move(-globalSpeed, 0);
     
     if (floor[0].getPosition().x <= -floorWidth)
	{
		floor[0].setPosition(0, floorHeightPosition);
		floor[1].setPosition(floorWidth, floorHeightPosition);

          road[0].setPosition(0, floorHeightPosition);
          road[1].setPosition(floorWidth, floorHeightPosition);
	}
     

	// Roadrunner jump
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && rrSprite.getPosition().y >= 505)
	{
		jumpStatus = 1;
	}

	if (jumpStatus == 1 && (rrSprite.getPosition().y <= 505))
	{
		rrSprite.move(0, -15);
	}

	if (rrSprite.getPosition().y <= 150)
	{
		jumpStatus = 0;
	}

	if (jumpStatus == 0 && (rrSprite.getPosition().y < 505))
	{
		rrSprite.move(0, 15);
	}

     // Sanic jump
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sanicSprite.getPosition().y >= 485)
     {
          jumpStatus = 1;
     }

     if (jumpStatus == 1 && (sanicSprite.getPosition().y <= 485))
     {
          sanicSprite.move(0, -15);
     }

     if (sanicSprite.getPosition().y <= 150)
     {
          jumpStatus = 0;
     }

     if (jumpStatus == 0 && (sanicSprite.getPosition().y < 485))
     {
          sanicSprite.move(0, 15);
     }

	//boulder movement
     boulderSprite.move(-globalSpeed, 0);
     boulderx -= globalSpeed;

	if (boulderx <= -100)
	{
		boulderSprite.setPosition(boulderx = 1000, 480);
	}

     if (overlap(rrSprite, boulderSprite) && sanicPowerupStatus == 0)
	{
		eaten++;
		isoverlap = true;
		//	chari.setScale(0, 0);
	}

     // Death conditions
     if (isoverlap)
     {
          music.stop();

          if (deathSound)
          {
               sound.setBuffer(squawk);
               sound.play();
               deathSound = 0;
          }

     }
     
}



void draw(sf::RenderWindow& window, sf::RectangleShape floor[], sf::RectangleShape road[], sf::Sprite& rrSprite,
     sf::Sprite& boulderSprite, sf::Text &text, sf::Text &text2, bool isoverlap, sf::Sprite& sanicSprite, sf::Sprite& sanicPowerupSprite, bool sanicPowerupStatus)
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
          window.draw(road[0]);
          window.draw(road[1]);
          window.draw(sanicPowerupSprite);
		window.draw(boulderSprite);
          if (sanicPowerupStatus)
               window.draw(sanicSprite);     //draws sanic during his powerup phase
          else
               window.draw(rrSprite);        //otherwise draw roadrunner
		window.draw(text2);

	}

	window.display();
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 800), "RoadRunner");
	window.setVerticalSyncEnabled(true);

     //IMPORTANT FOR GAME TO BE CONSISTENT, use this whenever possible
     int globalSpeed = 15;

     //GENERATE RANDMONESS
     srand(time(NULL));

     // Music
     sf::Music music;
     if (!music.openFromFile(resourcePath() + "assets/roadrunner_theme.wav"))
          return -1;
     music.play();
     music.setLoop(true);

     // Sounds
     sf::SoundBuffer squawk;
     if (!squawk.loadFromFile(resourcePath() + "assets/chicken_squawk.wav"))
          return -1;
     sf::Sound sound;
     bool deathSound = 1;

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
	double pos = 505;
	bool jumpStatus = 0;
	rrSprite.setPosition(25, pos);

	//floor variables
	int floorWidth = window.getSize().x;                         // floorWidth is the size of the window
	int floorSize = 200;
	int floorHeightPosition = window.getSize().y - floorSize;
     int roadSize = 25;

     // Floor and road objects
	sf::Texture floorTexture;
     floorTexture.setRepeated(true);
	floorTexture.loadFromFile(resourcePath() + "assets/yellow_floor.jpg");
     sf::RectangleShape floor[2];

     sf::Texture roadTexture;
     roadTexture.setRepeated(true);
     roadTexture.loadFromFile(resourcePath() + "assets/road.jpg");
     sf::RectangleShape road[2];
     
     //assign floor/road properties
	for (int x = 0; x < 2; x++)
	{
		floor[x].setSize(sf::Vector2f(floorWidth, floorSize));
		floor[x].setTexture(&floorTexture);
		floor[x].setPosition(floorWidth * x, floorHeightPosition);

          road[x].setSize(sf::Vector2f(floorWidth, roadSize));
          road[x].setTexture(&roadTexture);
          road[x].setPosition(floorWidth * x, floorHeightPosition);
	}

	// boulder variables
	sf::Texture boulderTexture;
	boulderTexture.loadFromFile(resourcePath() + "assets/boulder.jpeg");
	sf::Sprite boulderSprite(boulderTexture);
	boulderSprite.setScale(0.7, 0.7);

	int boulderx = 1000;
	int bouldery = window.getSize().y + floorSize;

	boulderSprite.setPosition(boulderx, bouldery);

	// Overlap variables
	int eaten = 0;
	bool isoverlap = false;

     // Sanic object
     sf::Texture sanicTexture;
     sanicTexture.loadFromFile(resourcePath() + "assets/sanic.png");
     sf::Sprite sanicSprite(sanicTexture);

     // Sanic properties
     sanicSprite.setScale(0.15, 0.15);
     sanicSprite.setPosition(25, 485);

     // Sanicpowerup object
     sf::Texture sanicPowerupTexture;
     sanicPowerupTexture.loadFromFile(resourcePath() + "assets/sanic_powerup.png");
     sf::Sprite sanicPowerupSprite(sanicPowerupTexture);

     // Sanicpowerup properties
     sanicPowerupSprite.setScale(0.05, 0.05);
     sanicPowerupSprite.setPosition(-floorWidth, 300);

     // Powerup variables
     bool powerupSpawnStatus = false;
     bool sanicPowerupStatus = false;

     sf::Time powerupSpawnTimer = sf::seconds(10);     //how long it takes for the "FIRST" powerup to spawn
     sf::Time sanicTime;                               //how long sanic's powerup last

	while (window.isOpen())
	{
		handleEvent(window);
          draw(window, floor, road, rrSprite, boulderSprite, text, text2, isoverlap, sanicSprite, sanicPowerupSprite, sanicPowerupStatus);       //draw first
          update(floor, road, floorWidth, floorHeightPosition, rrSprite, pos, jumpStatus, isoverlap, ss, clock, text2,boulderSprite, eaten,      //update drawings
               boulderx, music, sound, squawk, deathSound, sanicSprite, sanicPowerupSprite, sanicPowerupStatus, globalSpeed, sanicTime, 
               powerupSpawnStatus, powerupSpawnTimer);
	}

	return 0;
}