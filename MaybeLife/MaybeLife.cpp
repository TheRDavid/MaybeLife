// MaybeLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/String.hpp>
#include <time.h>
#include <string>

#include "UI.h"
#include "Environment.h"
#include "Utilities.h"
#include "InputManager.h"
#include "Grid.h"
#include "AppConfig.h"

int main()
{
	srand(time(NULL));
	float sceneZoom = 1;
	std::cout << "MaybeLife starting up, oh boi!\n";
	sf::Vector2i envSize = sf::Vector2i(7680, 4320);
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	sf::View sceneView(sf::FloatRect(0, 0, envSize.x, envSize.y));
	sf::View uiView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
	window.setPosition(sf::Vector2i(0, 0));
	int numEntities = 10 * 1000, numZones = 100 * 1000, numThreads = 8;
	Entity::Behaviour defaultBehaviour = Entity::Behaviour::SPREAD;
	float xBoundarySize = .85, yBoundarySize = .85;
	int boundaryWidth = envSize.x * xBoundarySize, boundaryHeight = envSize.y * yBoundarySize;
	float boundaryXStart = (envSize.x - boundaryWidth) / 2, boundaryYStart = (envSize.y - boundaryHeight) / 2;
	std::vector<Entity*>* entities = new std::vector<Entity*>();
	entities->reserve(numEntities);
	Environment environment(&window, envSize, numZones, numThreads, &sceneView);
	for (int i = 0; i < numEntities; i++) {
		float s = 1 + rand() % 5;
		sf::Vector2f entitySize = sf::Vector2f(s, s);
		sf::Vector2f position;
		position = sf::Vector2f(boundaryXStart + (rand() % boundaryWidth), boundaryYStart + (rand() % boundaryHeight));
		entities->push_back(new Entity(&environment,
			defaultBehaviour,
			position,
			entitySize,
			true,
			sf::Color::White));
	}
	UI ui(&window, &environment, &uiView);
	int loopNr = 0;
	window.setFramerateLimit(30);
	InputManager inputManager(&environment, &window, &sceneView, &uiView);
	environment.start(entities);
	environment.renderRectPosition = sceneView.getCenter() - sf::Vector2f(sceneView.getSize().x / 2, sceneView.getSize().y / 2);
	environment.renderRectSize = sf::Vector2f(sceneView.getSize().x, sceneView.getSize().y);
	sf::Event event;
	while (window.isOpen())
	{
		if (window.pollEvent(event))
		{
			inputManager.handleEvents(event);
		}
		window.clear();
		environment.draw();
		if (environment.showUI)
		{
			ui.refresh();
		}
		window.display();
	}

	return 0;
}
