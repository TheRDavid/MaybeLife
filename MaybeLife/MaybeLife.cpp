#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/String.hpp>
#include <time.h>
#include <string>

#include "Environment.h"

#include "GoodGuy.h"
#include "BadGuy.h"
#include "Slave.h"
#include "Peasant.h"
#include "FoodSource.h"
#include "Base.h"

#include "Utilities.h"
#include "InputManager.h"
#include "Grid.h"
#include "AppConfig.h"
#include "SimConfig.h"

#include "GUI.h"

#include "GroupMonitorPanel.h"
#include "StatusPanel.h"
#include "RuntimeControlPanel.h"
#include "EntityInspectionPanel.h"
#include "RecordingPlaybackPanel.h"

int main()
{
#pragma warning( suppress : 4244 ) // we just want some kinda randomness, precision loss here not relevant
	srand(time(NULL));
	std::cout << "MaybeLife starting up, oh boi!\n";

	/////////////// START CONFIG ///////////////
	sf::Vector2i envSize = sf::Vector2i(1920, 1080);
	sf::RenderWindow m_window(sf::VideoMode(1920, 1080), "SFML works!", sf::Style::Titlebar | sf::Style::Close);

	int numGoodGuys = SimConfig::getInstance().getNumGoodGuys();
	int numBadGuys = SimConfig::getInstance().getNumBadGuys();
	int numPeasants = SimConfig::getInstance().getNumPeasants();
	int numSlaves = SimConfig::getInstance().getNumSlaves();

	int goodGuysStartNutrition = SimConfig::getInstance().getGoodGuysStartNutrition();
	int badGuysStartNutrition = SimConfig::getInstance().getBadGuysStartNutrition();
	int numFoodSources = SimConfig::getInstance().getNumFoodSources();

	sf::Vector2f goodBasePos = sf::Vector2f(300, 300);
	sf::Vector2f badBasePos = sf::Vector2f(500, 500);

	int numZones = SimConfig::getInstance().getNumZones();
	int numThreads = SimConfig::getInstance().getNumThreads();
	float xBoundarySize = .85f;
	float yBoundarySize = .85f;
	//////////////// END CONFIG ///////////////

	float boundaryWidth = envSize.x * xBoundarySize, boundaryHeight = envSize.y * yBoundarySize;
	float boundaryXStart = (envSize.x - boundaryWidth) / 2, boundaryYStart = (envSize.y - boundaryHeight) / 2;
	sf::View sceneView(sf::FloatRect(0.f, 0.f, (float)envSize.x, (float)envSize.y));
	sf::View uiView(sf::FloatRect(0.f, 0.f, (float)m_window.getSize().x, (float)m_window.getSize().y));
	sf::View guiView(sf::FloatRect(0.f, 0.f, (float)m_window.getSize().x, (float)m_window.getSize().y));

	m_window.setPosition(sf::Vector2i(0, 0));

	std::vector<std::shared_ptr<Entity>>* entities = new std::vector<std::shared_ptr<Entity>>();
	entities->reserve(2 + 100 + numGoodGuys + numPeasants + numBadGuys + numSlaves);
	Environment environment(&m_window, envSize, numZones, numThreads, &sceneView);

	std::shared_ptr<Base> goodBase = std::make_shared<Base>(&environment, goodBasePos, true, goodGuysStartNutrition);
	std::shared_ptr<Base> badBase = std::make_shared<Base>(&environment, badBasePos, false, badGuysStartNutrition);

	entities->push_back(
		goodBase
	);

	entities->push_back(
		badBase
	);

	for (int i = 0; i < numFoodSources; i++)
	{
		entities->push_back(
			std::make_shared<FoodSource>(&environment, sf::Vector2f((float)(rand() % environment.m_size.x), (float)(rand() % environment.m_size.y)))
		);
	}

	for (int i = 0; i < numGoodGuys; i++) {
		float posOffsetX = (float)(rand() % 200 - 100);
		float posOffsetY = (float)(rand() % 200 - 100);
		sf::Vector2f position = goodBasePos + sf::Vector2f(posOffsetX, posOffsetY);
		entities->push_back(
			std::make_shared<GoodGuy>(&environment, position, goodBase)
		);
	}

	for (int i = 0; i < numPeasants; i++) {
		float posOffsetX = (float)(rand() % 300 - 150);
		float posOffsetY = (float)(rand() % 300 - 150);
		sf::Vector2f position = goodBasePos + sf::Vector2f(posOffsetX, posOffsetY);
		entities->push_back(
			std::make_shared<Peasant>(&environment, position, goodBase)
		);
	}

	for (int i = 0; i < numBadGuys; i++) {
		float posOffsetX = (float)(rand() % 200 - 100);
		float posOffsetY = (float)(rand() % 200 - 100);
		sf::Vector2f position = badBasePos + sf::Vector2f(posOffsetX, posOffsetY);
		entities->push_back(
			std::make_shared<BadGuy>(&environment, position, badBase)
		);
	}

	for (int i = 0; i < numSlaves; i++) {
		float posOffsetX = (float)(rand() % 300 - 150);
		float posOffsetY = (float)(rand() % 300 - 150);
		sf::Vector2f position = badBasePos + sf::Vector2f(posOffsetX, posOffsetY);
		entities->push_back(
			std::make_shared<Slave>(&environment, position, badBase)
		);
	}


	m_window.setFramerateLimit(60);
	InputManager inputManager(&environment, &m_window, &sceneView, &uiView);
	environment.start(entities);
	environment.m_renderRectPosition = sceneView.getCenter() - sf::Vector2f(sceneView.getSize().x / 2, sceneView.getSize().y / 2);
	environment.m_renderRectSize = sf::Vector2f(sceneView.getSize().x, sceneView.getSize().y);
	sf::Event event;

	gui::GUI gui = gui::GUI(&m_window, &guiView);
	gui.m_mainPanel->addChild(new StatusPanel(&environment, &m_window));

	GroupMonitorPanel* gmp1 = new GroupMonitorPanel(&environment, &m_window, false, badBase);
	gmp1->m_position = sf::Vector2f(100, 800);
	GroupMonitorPanel* gmp0 = new GroupMonitorPanel(&environment, &m_window, true, goodBase);
	gmp0->m_position = sf::Vector2f(100, 400);

	RuntimeControlPanel* rntcp = new RuntimeControlPanel(&m_window, &environment);
	EntityInspectionPanel* eip = new EntityInspectionPanel(&m_window, sf::Vector2f(600, 600));
	RecordingPlaybackPanel* rpp = new RecordingPlaybackPanel(&m_window, sf::Vector2f(0, 300));

	gui.m_mainPanel->addChild(gmp0);
	gui.m_mainPanel->addChild(gmp1);
	gui.m_mainPanel->addChild(rntcp);
	gui.m_mainPanel->addChild(eip);
	gui.m_mainPanel->addChild(rpp);
	while (m_window.isOpen())
	{
		if (m_window.pollEvent(event))
		{
			inputManager.handleEvents(event);
			gui.handle(event);
		}
		m_window.clear();
		environment.draw();
		if (environment.m_showGUI)
		{
			gui.update();
		}
		m_window.display();
	}

	return 0;
}
