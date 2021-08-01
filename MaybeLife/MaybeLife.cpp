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
	sf::RenderWindow window(sf::VideoMode(2560, 1260), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Magenta);
	Vector2i viewPortCoordsBegin = Vector2i(-400, -400);
	Vector2i viewPortCoordsEnd = Vector2i(400, 400);
	InputManager inputManager;
	Environment environment(&window, Vector2i(2560, 1260), 450);
	int numEntities = 1000000;
	for (int i = 0; i < numEntities; i++) {
		/*
		environment.addEntity(
			new Entity(environment.nextEntityId++, Vector2f(
				environment.size.x / 5 + rand() % (environment.size.x / 5*3),
				environment.size.y / 5 + rand() % (environment.size.y / 5*3))));*/
		
				environment.addEntity(
			new Entity(environment.nextEntityId++, Vector2f(
				rand() % environment.size.x,
				rand() % environment.size.y)));
	}
	UI ui(&window, &environment);
	int loopNr = 0;
	while (window.isOpen())
	{
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
					window.setTitle("Outside World");
				}
				else
					window.setTitle(zone->toString());
			}
		}

		window.clear();
		environment.update();
		environment.draw(viewPortCoordsBegin, viewPortCoordsEnd);
		ui.refresh();
		window.display();
	}

	return 0;
}
