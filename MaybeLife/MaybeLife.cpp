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
	srand(time(NULL));
	float sceneZoom = 1;
	std::cout << "MaybeLife starting up, oh boi!\n";
	Vector2i envSize = Vector2i(7680, 4320);
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	View sceneView(FloatRect(0, 0, envSize.x, envSize.y));
	View uiView(FloatRect(0, 0, window.getSize().x, window.getSize().y));
	window.setPosition(Vector2i(0, 0));
	int numEntities = 1000 * 1000, numZones = 1000 * 1000, numThreads = 8;
	Entity::Behaviour defaultBehaviour = Entity::Behaviour::SPREAD;
	float xBoundarySize = .85, yBoundarySize = .85;
	int boundaryWidth = envSize.x * xBoundarySize, boundaryHeight = envSize.y * yBoundarySize;
	float boundaryXStart = (envSize.x - boundaryWidth) / 2, boundaryYStart = (envSize.y - boundaryHeight) / 2;
	vector<Entity*>* entities = new vector<Entity*>();
	entities->reserve(numEntities);
	Environment environment(&window, envSize, numZones, numThreads);
	for (int i = 0; i < numEntities; i++) {
		float s = 1;
		Vector2f entitySize = Vector2f(s, s);
		Vector2f position;
		position = Vector2f(boundaryXStart + (rand() % boundaryWidth), boundaryYStart + (rand() % boundaryHeight));
		entities->push_back(new Entity(&environment,
			defaultBehaviour,
			position,
			entitySize,
			true,
			Color::White));
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
				if (event.mouseButton.button == sf::Mouse::Button::Right) {
					sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
					startDragPos = window.mapPixelToCoords(pixelPos);
					dragging = true;
				}
				else if (event.mouseButton.button == sf::Mouse::Button::Middle) {
					sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
					Vector2f worldPos = window.mapPixelToCoords(pixelPos);
					Entity* entity = new Entity(&environment, defaultBehaviour, worldPos);
					environment.insertLock.lock();
					environment.zoneAt(worldPos)->entities.insert(std::pair<unsigned long long int, Entity*>(entity->id, entity));
					environment.insertLock.unlock();
				}
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
