#pragma once

#include "Element.h"

namespace gui
{
	class Panel : public Element
	{
	public:
		std::string m_name = "Panel";
		sf::Color m_backgroundColor;
		sf::Color m_edgeColor;

		int m_edgeThickness;

		Panel(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size, sf::Color backgroundColor, sf::Color edgeColor, int edgeThickness, bool draggable);

		virtual void drawSelf(sf::Vector2f relativePosition) override;
	};

}