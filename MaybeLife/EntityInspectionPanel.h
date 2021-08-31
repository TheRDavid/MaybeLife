#pragma once
#include "Panel.h"

#include <SFML/Graphics.hpp>

#include "Person.h"
#include "Worker.h"
#include "TextPanel.h"
#include "Scrollbar.h"

class EntityInspectionPanel :
	public gui::Panel
{
public:
	std::weak_ptr<Entity> m_entity;
	gui::TextPanel* m_textPanel;
	gui::Scrollbar* m_scrollbar;

	EntityInspectionPanel(sf::RenderWindow* window);

	virtual void drawSelf(sf::Vector2f relativePosition) override;

};

