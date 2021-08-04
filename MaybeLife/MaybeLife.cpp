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
#include <time.h>
#include <string>
using namespace sf;

int main()
{
	float sceneZoom = 1;
	std::cout << "MaybeLife starting up, oh boi!\n";
	Vector2i envSize = Vector2i(7680, 4320);
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	View sceneView(FloatRect(0, 0, envSize.x, envSize.y));
	View uiView(FloatRect(0, 0, window.getSize().x, window.getSize().y));
	window.setPosition(Vector2i(0, 0));
	int numEntities = 500 * 1000, numZones = 1000 * 1000, numThreads = 8, entitySize = 1;
	Entity::Behaviour defaultBehaviour = Entity::Behaviour::SPREAD;
	float boundarySize = 1;
	int boundaryWidth = envSize.x * boundarySize, boundaryHeight = envSize.y * boundarySize;
	float boundaryXStart = (envSize.x - boundaryWidth) / 2, boundaryYStart = (envSize.y - boundaryHeight) / 2;
	vector<Entity*>* entities = new vector<Entity*>();
	entities->reserve(numEntities);
	srand(time(NULL));
	Environment environment(&window, envSize, numZones, numThreads);
	for (int i = 0; i < numEntities; i++) {
		Vector2f position;
		if (boundarySize >= 1)
			position = Vector2f(rand() % envSize.x, rand() % envSize.y);
		else
			position = Vector2f(boundaryXStart + (rand() % boundaryWidth), boundaryYStart + (rand() % boundaryHeight));
		entities->push_back(new Entity(
			i, &environment,
			defaultBehaviour,
			position,
			Vector2f(entitySize, entitySize),
			Color(max(70, rand() % 255), max(70, rand() % 255), max(70, rand() % 255), 255)));
	}
	UI ui(&window, &environment);
	int loopNr = 0;
	Zone* zone = environment.zoneAt(Vector2f(0, 0));
	window.setFramerateLimit(30);
	InputManager inputManager(&environment);
	Vector2f startDragPos, endDragPos;
	bool dragging = false;
	float currZoom = 4;
	environment.start(entities);
	while (window.isOpen())
	{
		string titleString = environment.stepsToString();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
					sceneView.zoom(0.9);
					currZoom *= 0.9;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
					sceneView.zoom(1.1);
					currZoom *= 1.1;
				}
			}
			else if (event.type == sf::Event::MouseMoved) {
				sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

				// convert it to world coordinates
				sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
				zone = environment.zoneAt(worldPos);

				if (dragging) {
					sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

					// convert it to world coordinates
					sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
					endDragPos = worldPos;
					Vector2f delta = Vector2f((startDragPos.x - endDragPos.x) * currZoom, (startDragPos.y - endDragPos.y) * currZoom);
					sceneView.move(delta);
					startDragPos = endDragPos;
				}

			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
				startDragPos = window.mapPixelToCoords(pixelPos);
				dragging = true;

			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				dragging = false;

			}
		}
		if (zone == nullptr) {
			titleString += " Outside World";
		}
		else
			titleString += " " + zone->toString();
		window.setTitle(titleString);
		window.clear();
		if (numThreads == 0)
			environment.updateEntities(0, environment.numZones, -1);
		window.setView(sceneView);
		environment.draw();
		if (environment.showUI)
		{
			window.setView(uiView);
			ui.refresh();
		}
		window.display();
	}

	return 0;
}
