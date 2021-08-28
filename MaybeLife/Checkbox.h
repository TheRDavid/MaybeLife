#pragma once
#include "TextPanel.h"
namespace gui {
	class Checkbox :
		public TextPanel
	{
	public:
		bool m_checked;

		Checkbox(sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, sf::Color edgeColor,
			sf::Color textColor, std::string displayText, int fontSize, bool checked);

		virtual void drawSelf(sf::Vector2f relativePosition) override;
	protected:
		virtual void onMouseReleased(sf::Event event) override;
	};
}