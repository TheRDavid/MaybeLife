#pragma once
#include "Panel.h"

#include <SFML/Graphics.hpp>

#include "Person.h"
#include "Worker.h"
#include "TextPanel.h"

class EntityInspectionPanel :
	public gui::Panel
{
public:
	std::weak_ptr<Entity> m_entity;
	gui::TextPanel* m_textPanel;

	EntityInspectionPanel(sf::RenderWindow* window, sf::Vector2f position);

	virtual void drawSelf(sf::Vector2f relativePosition) override;
};

