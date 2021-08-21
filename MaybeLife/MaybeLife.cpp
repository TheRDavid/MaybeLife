// MaybeLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/String.hpp>
#include <time.h>
#include <string>

#include "Environment.h"
#include "Utilities.h"
#include "InputManager.h"
#include "Grid.h"
#include "AppConfig.h"
#include "SimConfig.h"

#include "GUI.h"

#include "StatusPanel.h"

int main()
{
	srand(time(NULL));
	std::cout << "MaybeLife starting up, oh boi!\n";

	/////////////// START CONFIG ///////////////
	sf::Vector2i envSize = sf::Vector2i(7680, 4320);
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	int numEntities = SimConfig::getInstance().getNumEntities();
	int numZones = SimConfig::getInstance().getNumZones();
	int numThreads = SimConfig::getInstance().getNumThreads();
	Entity::Behaviour defaultBehaviour = SimConfig::getInstance().getDefaultBehaviour();
	float xBoundarySize = .85;
	float yBoundarySize = .85;
	//////////////// END CONFIG ///////////////

	int boundaryWidth = envSize.x * xBoundarySize, boundaryHeight = envSize.y * yBoundarySize;
	float boundaryXStart = (envSize.x - boundaryWidth) / 2, boundaryYStart = (envSize.y - boundaryHeight) / 2;
	sf::View sceneView(sf::FloatRect(0, 0, envSize.x, envSize.y));
	sf::View uiView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
	sf::View guiView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));

	window.setPosition(sf::Vector2i(0, 0));

	std::vector<Entity*>* entities = new std::vector<Entity*>();
	entities->reserve(numEntities);
	Environment environment(&window, envSize, numZones, numThreads, &sceneView);
	for (int i = 0; i < numEntities; i++) {
		float s = 1 + rand() % 5;
		sf::Vector2f entitySize = sf::Vector2f(s, s);
		sf::Vector2f position;
		sf::Color entityColor = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255);
		//std::cout << "Color: " << ut::to_string(entityColor) << std::endl;
		position = sf::Vector2f(boundaryXStart + (rand() % boundaryWidth), boundaryYStart + (rand() % boundaryHeight));
		entities->push_back(new Entity(&environment,
			defaultBehaviour,
			position,
			entitySize,
			true,
			entityColor));
	}
	int loopNr = 0;
	window.setFramerateLimit(30);
	InputManager inputManager(&environment, &window, &sceneView, &uiView);
	environment.start(entities);
	environment.renderRectPosition = sceneView.getCenter() - sf::Vector2f(sceneView.getSize().x / 2, sceneView.getSize().y / 2);
	environment.renderRectSize = sf::Vector2f(sceneView.getSize().x, sceneView.getSize().y);
	sf::Event event;

	gui::GUI gui = gui::GUI(&window, &guiView, inputManager.commander);
	gui.mainPanel->addChild(new StatusPanel(&environment, &window));
	StatusPanel* sp = new StatusPanel(&environment, &window);
	sp->position = sf::Vector2f(100, 300);
	gui.mainPanel->addChild(sp);
	while (window.isOpen())
	{
		if (window.pollEvent(event))
		{
			inputManager.handleEvents(event);
			gui.handle(event);
		}
		window.clear();
		environment.draw();
		if (environment.showUI)
		{
			gui.update();
		}
		window.display();
	}

	return 0;
}
