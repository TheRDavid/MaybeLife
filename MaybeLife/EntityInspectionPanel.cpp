#include "EntityInspectionPanel.h"

#include "Commander.h"
#include "Peasant.h"
#include "Slave.h"
#include "json.hpp"

EntityInspectionPanel::EntityInspectionPanel(sf::RenderWindow * window)
	:gui::Panel(window, sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Color(22, 32, 22, 180), sf::Color::Yellow, 1, false)
{
	m_position = sf::Vector2f(m_window->getSize().x - 360, 0);
	m_size = sf::Vector2f(360, m_window->getSize().y);

	m_textPanel = new gui::TextPanel(window, sf::Vector2f(20, 20), sf::Vector2f(0, 0), sf::Color::Transparent, sf::Color::Transparent, 0, sf::Color::White, "Uninitialized", 13);
	m_scrollbar = new gui::Scrollbar(window, sf::Vector2f(335, 0), sf::Vector2f(25, m_size.y), 0, 100, 0);

	addChild(m_textPanel);
	addChild(m_scrollbar);
}

void EntityInspectionPanel::drawSelf(sf::Vector2f relativePosition)
{
	m_textPanel->m_childrenOffset = sf::Vector2f(0, m_scrollbar->m_value / (m_scrollbar->m_max - m_scrollbar->m_min));
	Panel::drawSelf(relativePosition);

	std::vector<std::weak_ptr<Entity>> selectedEntities = Commander::getInstance().getSelectedEntities();
	std::string text = "";
	std::map<std::string, int> typeOccurrences;

	if (selectedEntities.size() == 0)
	{
		m_entity = Commander::getInstance().getSelectedEntity();
		if (!m_entity.expired())
		{
			nlohmann::json rep;
			m_entity.lock()->jsonify(&rep);
			text = rep.dump(4);
		}
	}
	else {
		for (auto entity : selectedEntities)
		{
			if (!entity.expired())
			{
				nlohmann::json rep;
				entity.lock()->jsonify(&rep);
				if (typeOccurrences.find(rep["type"]) == typeOccurrences.end())
				{
					typeOccurrences[rep["type"]] = 1;
				}
				else {
					typeOccurrences[rep["type"]]++;
				}
				text += rep.dump(4) + "\n";
			}
		}
	}
	std::string summaryText = std::to_string(selectedEntities.size()) + " Entities\n";
	for (auto entry : typeOccurrences)
	{
		summaryText += "\n" + entry.first + "s: " + std::to_string(entry.second);
	}
	text = summaryText + "\n--------------------\n" + text;
	m_textPanel->setText(text);
	size_t lineCount = std::count_if(text.begin(), text.end(), [](char c) {return c == '\n'; });
	m_scrollbar->handleLength = std::min(1.0f, m_size.y / ((float)lineCount * 16));
	//std::cout << m_size.y << "/" << lineCount * 16 << " = " << m_scrollbar->handleLength << std::endl;
}
