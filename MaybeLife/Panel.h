#pragma once

#include "Element.h"

namespace gui
{
	class Panel : public Element
	{
	public:
		std::string name = "Panel";
		sf::Color backgroundColor;
		sf::Color edgeColor;

		int edgeThickness;

		Panel(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size, sf::Color backgroundColor, sf::Color edgeColor, int edgeThickness, bool draggable);

		virtual void drawSelf(sf::Vector2f relativePosition) override;
	};

}