#include "EntityInspectionPanel.h"

#include "Commander.h"
#include "Peasant.h"
#include "Slave.h"
#include "json.hpp"

EntityInspectionPanel::EntityInspectionPanel(sf::RenderWindow * window)
	:gui::Panel(window, sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Color(22, 32, 22, 180), sf::Color::Yellow, 1, true)
{
	m_position = sf::Vector2f(m_window->getSize().x - 360, 0);
	m_size = sf::Vector2f(360, m_window->getSize().y);
	m_textPanel = new gui::TextPanel(window, sf::Vector2f(20, 20), sf::Vector2f(0, 0), sf::Color::Transparent, sf::Color::Transparent, 0, sf::Color::White, "Uninitialized", 13);
	addChild(m_textPanel);
}

void EntityInspectionPanel::drawSelf(sf::Vector2f relativePosition)
{
	Panel::drawSelf(relativePosition);
	std::string text = "Uninitialized";

	std::vector<std::weak_ptr<Entity>> selectedEntities = Commander::getInstance().getSelectedEntities();
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
				text += rep.dump(4) + "\n";
			}
		}
	}
	m_textPanel->setText(text);
}
