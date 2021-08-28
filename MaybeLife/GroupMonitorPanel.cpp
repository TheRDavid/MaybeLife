#include "GroupMonitorPanel.h"

#include <vector>

#include "Grid.h"
#include "Entity.h"
#include "Base.h"
#include "Environment.h"
#include "Worker.h"
#include "Fighter.h"
#include "GoodGuy.h"
#include "Peasant.h"
#include "Slave.h"
#include "BadGuy.h"
#include "TextPanel.h"
#include "Commander.h"
#include "GUI.h"

GroupMonitorPanel::GroupMonitorPanel(Environment * environment, sf::RenderWindow * window, bool good, std::shared_ptr<Base> base)
	: Panel(
		window,
		sf::Vector2f(40, 40),
		sf::Vector2f(330, 200),
		sf::Color(22, 32, 22, 200),
		base->m_color,
		1,
		true), m_good(good), m_base(base)
{
	this->m_environment = environment;
	m_statusText = new gui::TextPanel(
		window,
		sf::Vector2f(20, 15),
		sf::Vector2f(0, 0),
		sf::Color::Transparent,
		sf::Color::Transparent,
		0,
		base->m_color,
		"Uninitialized",
		16
	);

	action spawnFighter = [this](sf::Event event) {
		this->spawnFighter(event);
	};

	action spawnWorker = [this](sf::Event event) {
		this->spawnWorker(event);
	};

	m_spawnFighterButton = new gui::Button(
		window, sf::Vector2f(25, 160), sf::Vector2f(130, 25), "Spawn Fighter",
		spawnFighter, base->m_color
	);

	m_spawnWorkerButton = new gui::Button(
		window, sf::Vector2f(170, 160), sf::Vector2f(130, 25), "Spawn Worker",
		spawnWorker, base->m_color
	);

	addChild(m_statusText);
	addChild(m_spawnFighterButton);
	addChild(m_spawnWorkerButton);
	m_name = "GroupMonitorPanel";
}


inline std::string shorthand(int num) {
	if (num <= 1000) {
		return std::to_string(num);
	}
	float shorthandCount;
	std::string multiplierString = "";
	if (num > 1000 * 1000 * 1000) {

#pragma warning( suppress : 4244 ) // we don't care about double vs float precision here, quite the opposite
		shorthandCount = (float)num / (1000.0 * 1000.0 * 1000.0);
		multiplierString = " B";
	}
	else if (num > 1000 * 1000) {
#pragma warning( suppress : 4244 ) // we don't care about double vs float precision here, quite the opposite
		shorthandCount = (float)num / (1000.0 * 1000.0);
		multiplierString = " M";
	}
	else if (num > 1000) {
#pragma warning( suppress : 4244 ) // we don't care about double vs float precision here, quite the opposite
		shorthandCount = (float)num / (1000.0);
		multiplierString = " K";
	}
	std::string countText = std::to_string(shorthandCount).substr(0, 5);
	countText.erase(countText.find_last_not_of('0') + 1, std::string::npos);
	countText.erase(countText.find_last_not_of('.') + 1, std::string::npos);
	return countText + multiplierString;
}

void GroupMonitorPanel::drawSelf(sf::Vector2f relativePosition)
{
	float nutritionAtBase = m_base->m_nutrition;
	float avgCarriedNutrition = 0, avgHealth = 0;
	int numWorkers = 0, numFighters = 0, numMembers = 0;

	for (auto entity : *m_environment->m_entities)
	{
		if (auto person = std::dynamic_pointer_cast<Person>(entity))
		{
			if (person->m_good == m_good)
			{
				avgHealth += person->m_health;
				numMembers++;
				if (auto fighter = std::dynamic_pointer_cast<Fighter>(entity))
				{
					numFighters++;
				}

				if (auto worker = std::dynamic_pointer_cast<Worker>(entity))
				{
					avgCarriedNutrition += worker->m_gatheredNutrition;
					numWorkers++;
				}
			}
		}
	}
	avgHealth /= numMembers;
	avgCarriedNutrition /= numWorkers;
	if (m_good)
	{
		Commander::getInstance().m_numGoodGuys = numMembers;
	}
	else {
		Commander::getInstance().m_numBadGuys = numMembers;
	}

	if (true || AppConfig::getInstance().m_showFPS) {
		std::string groupText = m_good ? "Good Guys" : "Bad Guys";
		m_statusText->setText(groupText + "\nNutrition at base: " + std::to_string(nutritionAtBase)
			+ "\nMembers: " + std::to_string(numMembers)
			+ "\nWorkers: " + std::to_string(numWorkers)
			+ "\nFighters: " + std::to_string(numFighters)
			+ "\nAvg Health: " + std::to_string(avgHealth)
			+ "\nAvg Carry: " + std::to_string(avgCarriedNutrition)
		);
	}
	Panel::drawSelf(relativePosition);
}

void GroupMonitorPanel::spawnFighter(sf::Event event)
{
	std::shared_ptr<Fighter> newFighter;
	if (m_good)
	{
		newFighter = std::make_shared<GoodGuy>(m_environment, m_base->m_position, m_base);
	}
	else {
		newFighter = std::make_shared<BadGuy>(m_environment, m_base->m_position, m_base);
	}

	Commander::getInstance().addEntity(
		newFighter
	);
}

void GroupMonitorPanel::spawnWorker(sf::Event event)
{
	std::shared_ptr<Worker> newWorker;

	if (m_good)
	{
		newWorker = std::make_shared<Peasant>(m_environment, m_base->m_position, m_base);
	}
	else {
		newWorker = std::make_shared<Slave>(m_environment, m_base->m_position, m_base);
	}

	Commander::getInstance().addEntity(
		newWorker
	);
}
