#include "EntityInspectionPanel.h"

#include "Commander.h"
#include "Peasant.h"
#include "Slave.h"

EntityInspectionPanel::EntityInspectionPanel(sf::RenderWindow * window, sf::Vector2f position)
	:gui::Panel(window, position, sf::Vector2f(240, 180), sf::Color(22, 32, 22, 220), sf::Color::Yellow, 1, true)
{
	m_textPanel = new gui::TextPanel(window, sf::Vector2f(20, 20), sf::Vector2f(0, 0), sf::Color::Transparent, sf::Color::Transparent, 0, sf::Color::White, "Uninitialized", 14);
	addChild(m_textPanel);
}

void EntityInspectionPanel::drawSelf(sf::Vector2f relativePosition)
{
	Panel::drawSelf(relativePosition);
	std::string text = "Uninitialized";
	m_entity = Commander::getInstance().getSelectedEntity();
	if (!m_entity.expired())
	{
		auto entity = m_entity.lock();
		text = "ID: " + std::to_string(entity->m_id) + "\n"
			+ "Enabled: " + std::to_string(entity->m_enabled) + "\n"
			+ "Name: " + entity->m_name + "\n";

		if (auto person = std::dynamic_pointer_cast<Person>(entity))
		{
			std::string group = (person->m_good ? "Good" : "Bad");
			std::string baseGroup = (person->m_base->m_good ? "Good" : "Bad");
			text += "PGroup: " + group + "\n"
				+ "Health: " + std::to_string(person->m_health) + "\n"
				+ "Base: " + baseGroup + "\n"
				+ "In view: " + std::to_string(person->m_inViewDistance.size()) + "\n";
			if (auto worker = std::dynamic_pointer_cast<Worker>(person))
			{
				group = (worker->m_good ? "Good" : "Bad");
				text += "Gathered: " + std::to_string(worker->m_gatheredNutrition) + "\n";
				text += "WGroup: " + group + "\n";

				if (auto peasant = std::dynamic_pointer_cast<Peasant>(worker))
				{
					group = (peasant->m_good ? "Good" : "Bad");
					text += "Peasant Group: " + group + "\n";
				}
				else if (auto slave = std::dynamic_pointer_cast<Slave>(worker))
				{
					group = (slave->m_good ? "Good" : "Bad");
					text += "Slave Group: " + group + "\n";
				}
				
			}
		}
	}
	m_textPanel->setText(text);
}
