#include "StatusPanel.h"

#include <memory>

#include "Grid.h"
#include "Entity.h"
#include "Person.h"
#include "Commander.h"

StatusPanel::StatusPanel(Environment * environment, sf::RenderWindow * window)
	: Panel(
		window,
		sf::Vector2f(40, 40),
		sf::Vector2f(250, 190),
		sf::Color(22, 32, 22, 220),
		sf::Color(255, 255, 255, 255),
		1,
		true)
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
		14
	);
	addChild(m_statusText);
	m_name = "StatusPanel";
}


inline std::string shorthand(int num) {
	if (num <= 1000) {
		return std::to_string(num);
	}
	float shorthandCount;
	std::string multiplierString = "";
	if (num > 1000 * 1000 * 1000) {
		shorthandCount = (float)num / (1000.0f * 1000.0f * 1000.0f);
		multiplierString = " B";
	}
	else if (num > 1000 * 1000) {
		shorthandCount = (float)num / (1000.0f * 1000.0f);
		multiplierString = " M";
	}
	else if (num > 1000) {
		shorthandCount = (float)num / (1000.0f);
		multiplierString = " K";
	}
	std::string countText = std::to_string(shorthandCount).substr(0, 5);
	countText.erase(countText.find_last_not_of('0') + 1, std::string::npos);
	countText.erase(countText.find_last_not_of('.') + 1, std::string::npos);
	return countText + multiplierString;
}

void StatusPanel::drawSelf(sf::Vector2f relativePosition)
{
	if (m_drawCallsSince++ == m_fpsAvgSpan) {
		int ms = std::max<long long>(1, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_fpsstart).count());
		m_fpsString = std::to_string((float)(m_fpsAvgSpan * 100000 / ms) / 100);
		m_fpsstart = std::chrono::high_resolution_clock::now();
		m_drawCallsSince = 0;
	}
	if (m_environment->steps - m_stepCountLast >= m_updateAvgSpan) {
		int ms = std::max<long long>(1, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_upstart).count());
		m_utString = std::to_string((float)(m_updateAvgSpan * 100000 / ms) / 100);
		m_upstart = std::chrono::high_resolution_clock::now();
		m_stepCountLast = m_environment->steps;
	}
	m_fpsString.erase(m_fpsString.find_last_not_of('0') + 1, std::string::npos);
	m_utString.erase(m_utString.find_last_not_of('0') + 1, std::string::npos);

	size_t totalInViewDistanceReferences = 0;

	for (auto entity : *m_environment->m_entities)
	{
		if (auto person = std::dynamic_pointer_cast<Person>(entity))
		{
			totalInViewDistanceReferences += person->m_inViewDistance.size();
		}
	}

	if (true || AppConfig::getInstance().m_showFPS) {
		m_statusText->setText(
			"Timestep: " + std::to_string(m_environment->steps) + "\n"
			"Entities: " + shorthand(m_environment->m_entities->size()) + "\n"
			+ "Zones: " + shorthand(m_environment->m_entityGrid->m_numZones) + "\n"
			+ "Threads: " + std::to_string(m_environment->m_numThreads) + "\n"
			+ "FPS: " + m_fpsString + "\n"
			+ "UPS: " + m_utString + "\n"
			+ "View Refs: " + shorthand(totalInViewDistanceReferences) + "\n"
			+ "Mouse (pixel): " + ut::to_string(Commander::getInstance().getMousePixelPosition()) + "\n"
			+ "Mouse (world): " + ut::to_string_integers(Commander::getInstance().getMouseWorldPosition()) + "\n"
		);
	}
	Panel::drawSelf(relativePosition);
}
