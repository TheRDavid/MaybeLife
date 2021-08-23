#include "GroupMonitorPanel.h"

#include <vector>

#include "Grid.h"
#include "Entity.h"
#include "Base.h"
#include "Environment.h"
#include "Worker.h"
#include "Fighter.h"


GroupMonitorPanel::GroupMonitorPanel(Environment * environment, sf::RenderWindow * window, bool good, std::shared_ptr<Base> base)
	: Panel(
		window,
		sf::Vector2f(40, 40),
		sf::Vector2f(450, 240),
		sf::Color(22, 32, 22, 220),
		sf::Color(255, 255, 255, 255),
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
		sf::Color(225, 225, 255, 255),
		"Uninitialized",
		24
	);
	addChild(m_statusText);
	m_name = "GroupMonitorPanel";
}


inline std::string shorthand(int num) {
	if (num <= 1000) {
		return std::to_string(num);
	}
	float shorthandCount;
	std::string multiplierString = "";
	if (num > 1000 * 1000 * 1000) {
		shorthandCount = (float)num / (1000.0 * 1000.0 * 1000.0);
		multiplierString = " B";
	}
	else if (num > 1000 * 1000) {
		shorthandCount = (float)num / (1000.0 * 1000.0);
		multiplierString = " M";
	}
	else if (num > 1000) {
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
	float avgCarriedNutrition = 0, numWorkers = 0, numFighters = 0, avgHealth = 0, numMembers = 0;

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
