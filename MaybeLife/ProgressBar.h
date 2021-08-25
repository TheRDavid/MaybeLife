#pragma once
#include "Panel.h"
namespace gui {
	class ProgressBar :
		public Panel
	{
	public:
		sf::Color m_fillColor;
		float m_value;
		ProgressBar(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size, float value, sf::Color fillColor, sf::Color backgroundColor);
		virtual void drawSelf(sf::Vector2f relativePosition) override;
	};

}