// MaybeLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/String.hpp>
#include "MaybeLife.h"
#include "InputManager.h"
#include "AppConfig.h"
#include "UI.h"
#include "Environment.h"
#include <string>
using namespace sf;
using namespace std;

int main()
{
	std::cout << "MaybeLife starting up, oh boi!\n";
	Vector2i envSize = Vector2i(1920, 1080);
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	window.setPosition(Vector2i(0, 0));
	Vector2i viewPortCoordsBegin = Vector2i(-400, -400);
	Vector2i viewPortCoordsEnd = Vector2i(400, 400);
	InputManager inputManager;
	int numEntities = 1000 * 1000, numZones = 1000 * 1000, numThreads = 4;
	Environment environment(&window, Vector2i(3400, 1340), numZones, numThreads, numEntities/numZones);
	Entities* entities = new Entities(numEntities);
	for (int i = 0; i < numEntities; i++) {
		//entities->push_back(new Entity(i, Vector2f(rand() % envSize.x, rand() % envSize.y), Vector2f(1, 1)));
		entities->push_back(new Entity(i, Vector2f(((rand() % envSize.x)/2) + (envSize.x / 4), ((rand() % envSize.y)/2)+(envSize.y/4)), Vector2f(1, 1)));
	}
	Environment environment(&window, envSize, numZones, numThreads, entities, Environment::Behaviour::SPREAD);
	UI ui(&window, &environment);
	int loopNr = 0;
	while (window.isOpen())
	{
		string titleString = environment.stepsToString();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::TextEntered) {
				inputManager.catchInput();
			}
			else if (event.type == sf::Event::MouseMoved) {
				sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

				// convert it to world coordinates
				sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
				Zone* zone = environment.zoneAt(worldPos);

				if (zone == nullptr) {
					titleString += " Outside World";
				}
				else
					titleString += " " + zone->toString();
			}
		}
		window.setTitle(titleString);
		window.clear();
		if (numThreads == 0)
			environment.updateEntities(0, environment.numZones, -1);
		environment.draw();
		ui.refresh();
		window.display();
	}

	return 0;
}
