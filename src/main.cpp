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
     bool& isoverlap, std::stringstream &ss, sf::Clock &clock, sf::Text &textTime, sf::Music& roadrunnerMusic, sf::Music& sanicMusic, sf::Sound& squawkSound, 
     bool& deathStatus, sf::Sprite& sanicSprite, sf::Sprite& sanicPowerupSprite, bool& sanicPowerupStatus, int& globalSpeed, sf::Time& sanicTime, bool& powerupSpawnStatus, 
     sf::Time& powerupSpawnTimer, sf::Sprite arrayOfBoulderObjectsSprite[], bool& boulderSpawnStatus, int numBoulder, int objStop)
{
	//game timer
     if (!isoverlap)
	{
		sf::Time time1 = clock.getElapsedTime();
		ss.str(std::string());
		ss << setprecision(2) << fixed << static_cast<double>(time1.asSeconds());
		textTime.setString(ss.str().c_str());
	}

     // Return to roadrunner character
     if (clock.getElapsedTime() >= sanicTime && sanicPowerupStatus)
     {
          sanicMusic.pause();
          roadrunnerMusic.play();
          sanicPowerupStatus = false;
     }

     // How fast everything moves
     if (sanicPowerupStatus && (clock.getElapsedTime() <= sanicTime - sf::seconds(1)))         //sanic run speed
          globalSpeed = 75;
     else
          globalSpeed = 15;                                                                    //roadrunner run speed

     // Powerup spawner, checks every 10 seconds
     if (clock.getElapsedTime() >= powerupSpawnTimer)
     {
          powerupSpawnTimer += sf::seconds(10);                       //time to spawn for every consecutive powerup after the first
          powerupSpawnStatus = 1;
     }

     // Powerup spawn chance
     if (powerupSpawnStatus)
     {
          if (rand() % 101 <= 33)                                 // 33% chance to spawn
          {
               sanicPowerupSprite.setScale(0.05, 0.05);
               sanicPowerupSprite.setPosition(floorWidth, 300);
          }
          powerupSpawnStatus = false;
     }

     // Sanicpowerup movement, how fast the icon moves
     if (sanicPowerupSprite.getPosition().x >= objStop)
          sanicPowerupSprite.move(-globalSpeed, 0);

     // Roadrunner and sanicpowerup collision
     if (overlap(rrSprite, sanicPowerupSprite) && !sanicPowerupStatus)
     {
          roadrunnerMusic.pause();
          sanicMusic.play();
          sanicPowerupStatus = true;
          sanicTime = clock.getElapsedTime() + sf::seconds(10);
          sanicPowerupSprite.setScale(0, 0);
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
		jumpStatus = true;
	}

	if (jumpStatus && (rrSprite.getPosition().y <= 505))
	{
		rrSprite.move(0, -15);
	}

	if (rrSprite.getPosition().y <= 150)
	{
		jumpStatus = false;
	}

	if (!jumpStatus && (rrSprite.getPosition().y < 505))
	{
		rrSprite.move(0, 15);
	}

     // Sanic jump
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sanicSprite.getPosition().y >= 485)
     {
          jumpStatus = true;
     }

     if (jumpStatus && (sanicSprite.getPosition().y <= 485))
     {
          sanicSprite.move(0, -15);
     }

     if (sanicSprite.getPosition().y <= 150)
     {
          jumpStatus = false;
     }

     if (!jumpStatus && (sanicSprite.getPosition().y < 485))
     {
          sanicSprite.move(0, 15);
     }

     // Movement of boulder
     for (int i = 0; i < numBoulder; i++)
     {
          if (arrayOfBoulderObjectsSprite[i].getPosition().x >= objStop)
               arrayOfBoulderObjectsSprite[i].move(-globalSpeed, 0);
     }

     // First checkpoint: spawns one random boulder
     for (int i = 0; i < numBoulder; i++)
     {
          if ((arrayOfBoulderObjectsSprite[i].getPosition().x >= 900 && arrayOfBoulderObjectsSprite[i].getPosition().x <= 1000) && boulderSpawnStatus)
          {
               boulderSpawnStatus = false;
               int reroll = true;
               int randBoulderSpawn = floorWidth + 600 + rand() % 1000;

               while (reroll)
               {
                    int randBoulder = rand() % numBoulder;

                    if (arrayOfBoulderObjectsSprite[randBoulder].getPosition().x <= objStop)
                    {
                         arrayOfBoulderObjectsSprite[randBoulder].setPosition(randBoulderSpawn, 500);
                         reroll = false;
                    }
               }
          }
     }

     // Second Checkpoint: renable spawning of boulders
     for (int i = 0; i < numBoulder; i++)
     {
          if (arrayOfBoulderObjectsSprite[i].getPosition().x >= 800 && arrayOfBoulderObjectsSprite[i].getPosition().x < 900)
               boulderSpawnStatus = true;
     }

     // Check collision for all boulders
     for (int i = 0; i < numBoulder; i++)
     {
          if (overlap(rrSprite, arrayOfBoulderObjectsSprite[i]) && !sanicPowerupStatus)
          {
               isoverlap = true;
          }
     }

     // Death conditions
     if (isoverlap && deathStatus)
     {
          roadrunnerMusic.pause();
          squawkSound.play();
          deathStatus = false;

     }
     
}



void draw(sf::RenderWindow& window, sf::RectangleShape floor[], sf::RectangleShape road[], sf::Sprite& rrSprite, sf::Text &text, 
     sf::Text &textTime, bool isoverlap, sf::Sprite& sanicSprite, sf::Sprite& sanicPowerupSprite, bool sanicPowerupStatus, 
     sf::Sprite arrayOfBoulderObjectsSprite[], int numBoulder)
{
	window.clear();

	if (isoverlap)
	{
		window.draw(text);
		window.draw(textTime);
	}
	else
	{
		window.draw(floor[0]);
		window.draw(floor[1]);
          window.draw(road[0]);
          window.draw(road[1]);
          window.draw(sanicPowerupSprite);
          for (int i = 0; i < numBoulder; i++)
          {
               window.draw(arrayOfBoulderObjectsSprite[i]);
          }
          if (sanicPowerupStatus)
               window.draw(sanicSprite);     //draws sanic during his powerup phase
          else
               window.draw(rrSprite);        //otherwise draw roadrunner
		window.draw(textTime);

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

     //STOP POINT FOR ALL OBJECTS
     int objStop = -500;

     // Music
     sf::Music roadrunnerMusic;
     roadrunnerMusic.openFromFile(resourcePath() + "assets/roadrunner_theme.wav");
     roadrunnerMusic.play();
     roadrunnerMusic.setLoop(true);

     sf::Music sanicMusic;
     sanicMusic.openFromFile(resourcePath() + "assets/sanic_theme.wav");
     sanicMusic.setLoop(true);
     sanicMusic.setVolume(15);

     // Sounds
     sf::SoundBuffer squawk;
     squawk.loadFromFile(resourcePath() + "assets/chicken_squawk.wav");
     sf::Sound squawkSound;
     squawkSound.setBuffer(squawk);
     bool deathStatus = true;

	// Time
	sf::Clock clock;
	sf::Time time1 = clock.getElapsedTime();
	sf::Time total;

	// Display time
	std::stringstream ss;
	ss.str(std::string());
	ss << time1.asSeconds();
	sf::Text textTime;
	textTime.setString(ss.str().c_str());

	// Load and set game time font
	sf::Font font2;
	textTime.setFont(font2);
	font2.loadFromFile(resourcePath() + "assets/Pacifico.ttf");
	textTime.setCharacterSize(100);
	textTime.setStyle(sf::Text::Bold);
	textTime.setColor(sf::Color::Blue);
	textTime.setPosition(0, 0);

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
	bool jumpStatus = false;
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

	// boulder object
	sf::Texture boulderTexture;
	boulderTexture.loadFromFile(resourcePath() + "assets/boulder.png");
     sf::Sprite arrayOfBoulderObjectsSprite[6];
     int numBoulder = 6;
     bool boulderSpawnStatus = true;

     // boulder properties
     for (int i = 0; i < numBoulder; i++)
     {
          arrayOfBoulderObjectsSprite[i].setTexture(boulderTexture);
          arrayOfBoulderObjectsSprite[i].setScale(0.3, 0.3);
          arrayOfBoulderObjectsSprite[i].setPosition(objStop, 500);                          // Spawn boulders off screen at stop point
     }

     // Randomly spawn first boulder and set position
     int randBoulder = rand() % numBoulder;
     int randBoulderSpawn = floorWidth + 600 + rand() % 1000;
     arrayOfBoulderObjectsSprite[randBoulder].setPosition(randBoulderSpawn, 500);


	// Overlap variables
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
     sanicPowerupSprite.setPosition(objStop, 300);                                        // Spawn sanicPowerup offscreen at stop point

     // Powerup variables
     bool powerupSpawnStatus = false;
     bool sanicPowerupStatus = false;

     sf::Time powerupSpawnTimer = sf::seconds(10);     //how long it takes for the "FIRST" powerup to spawn
     sf::Time sanicTime;                               //how long sanic's powerup last

	while (window.isOpen())
	{
		handleEvent(window);

          //draw first
          draw(window, floor, road, rrSprite, text, textTime, isoverlap, sanicSprite, sanicPowerupSprite, sanicPowerupStatus, arrayOfBoulderObjectsSprite, numBoulder);      

          //update drawings
          update(floor, road, floorWidth, floorHeightPosition, rrSprite, pos, jumpStatus, isoverlap, ss, clock, textTime, 
               roadrunnerMusic, sanicMusic, squawkSound, deathStatus, sanicSprite, sanicPowerupSprite, sanicPowerupStatus, globalSpeed, 
               sanicTime, powerupSpawnStatus, powerupSpawnTimer, arrayOfBoulderObjectsSprite, boulderSpawnStatus, numBoulder, objStop);
	}

	return 0;
}