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
#include "Utilities.h"
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
	int numEntities = 1000 * 1000, numZones = 100 * 1000, numThreads = 8;
	Entity::Behaviour defaultBehaviour = Entity::Behaviour::SPREAD;
	float xBoundarySize = .85, yBoundarySize = .85;
	int boundaryWidth = envSize.x * xBoundarySize, boundaryHeight = envSize.y * yBoundarySize;
	float boundaryXStart = (envSize.x - boundaryWidth) / 2, boundaryYStart = (envSize.y - boundaryHeight) / 2;
	vector<Entity*>* entities = new vector<Entity*>();
	entities->reserve(numEntities);
	Environment environment(&window, envSize, numZones, numThreads);
	for (int i = 0; i < numEntities; i++) {
		float s = 1 + rand() % 5;
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
	window.setFramerateLimit(30);
	InputManager inputManager(&environment, &window, &sceneView, &uiView);
	environment.start(entities);
	environment.renderRectPosition = sceneView.getCenter() - Vector2f(sceneView.getSize().x / 2, sceneView.getSize().y / 2);
	environment.renderRectSize = Vector2f(sceneView.getSize().x, sceneView.getSize().y);
	while (window.isOpen())
	{
		inputManager.handleEvents();
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
