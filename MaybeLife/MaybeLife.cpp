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
	sf::RenderWindow m_window(sf::VideoMode(1920, 1080), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	int numEntities = SimConfig::getInstance().getNumEntities();
	int numZones = SimConfig::getInstance().getNumZones();
	int numThreads = SimConfig::getInstance().getNumThreads();
	float xBoundarySize = .85;
	float yBoundarySize = .85;
	//////////////// END CONFIG ///////////////

	int boundaryWidth = envSize.x * xBoundarySize, boundaryHeight = envSize.y * yBoundarySize;
	float boundaryXStart = (envSize.x - boundaryWidth) / 2, boundaryYStart = (envSize.y - boundaryHeight) / 2;
	sf::View sceneView(sf::FloatRect(0, 0, envSize.x, envSize.y));
	sf::View uiView(sf::FloatRect(0, 0, m_window.getSize().x, m_window.getSize().y));
	sf::View guiView(sf::FloatRect(0, 0, m_window.getSize().x, m_window.getSize().y));

	m_window.setPosition(sf::Vector2i(0, 0));

	std::vector<Entity*>* entities = new std::vector<Entity*>();
	entities->reserve(numEntities);
	Environment environment(&m_window, envSize, numZones, numThreads, &sceneView);
	for (int i = 0; i < numEntities; i++) {
		float s = 1 + rand() % 5;
		sf::Vector2f entitySize = sf::Vector2f(s, s);
		sf::Vector2f position;
		sf::Color entityColor = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255);
		//std::cout << "Color: " << ut::to_string(entityColor) << std::endl;
		position = sf::Vector2f(boundaryXStart + (rand() % boundaryWidth), boundaryYStart + (rand() % boundaryHeight));
		entities->push_back(new Entity(&environment,
			position,
			entitySize,
			true,
			entityColor));
	}
	int loopNr = 0;
	m_window.setFramerateLimit(30);
	InputManager inputManager(&environment, &m_window, &sceneView, &uiView);
	environment.start(entities);
	environment.m_renderRectPosition = sceneView.getCenter() - sf::Vector2f(sceneView.getSize().x / 2, sceneView.getSize().y / 2);
	environment.m_renderRectSize = sf::Vector2f(sceneView.getSize().x, sceneView.getSize().y);
	sf::Event event;

	gui::GUI gui = gui::GUI(&m_window, &guiView, inputManager.m_commander);
	gui.m_mainPanel->addChild(new StatusPanel(&environment, &m_window));
	StatusPanel* sp = new StatusPanel(&environment, &m_window);
	sp->m_position = sf::Vector2f(100, 300);
	gui.m_mainPanel->addChild(sp);
	while (m_window.isOpen())
	{
		if (m_window.pollEvent(event))
		{
			inputManager.handleEvents(event);
			gui.handle(event);
		}
		m_window.clear();
		environment.draw();
		if (environment.m_showUI)
		{
			gui.update();
		}
		m_window.display();
	}

	return 0;
}
