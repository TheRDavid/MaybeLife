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

int main()
{
	std::cout << "MaybeLife starting up, oh boi!\n";
	sf::RenderWindow window(sf::VideoMode(3400, 1340), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	window.setPosition(Vector2i(0, 0));
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Magenta);
	Vector2i viewPortCoordsBegin = Vector2i(-400, -400);
	Vector2i viewPortCoordsEnd = Vector2i(400, 400);
	InputManager inputManager;
	int numEntities = 1000 * 1000, numZones = 1000000, numThreads = 4;
	Environment environment(&window, Vector2i(3400, 1340), numZones, numThreads, numEntities/numZones);
	Entities* entities = new Entities(numEntities);
	for (int i = 0; i < numEntities; i++) {
		entities->add(Vector2f(rand() % environment.size.x, rand() % environment.size.y), Vector2f(1, 1), Color::White);
		//entities->add(Vector2f(environment.size.x / 5 + rand() % environment.size.x / 5 * 3, environment.size.y / 5 + rand() % environment.size.y / 5 * 3), Vector2f(1, 1), Color::White);
	}
	environment.setEntities(entities);
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
			environment.entitiesDoSpread(0, environment.numZones, -1);
		environment.draw();
		ui.refresh();
		window.display();
	}

	return 0;
}
