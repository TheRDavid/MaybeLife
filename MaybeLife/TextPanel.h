#pragma once
#include "Panel.h"

#include "AppConfig.h"

using namespace gui;
namespace gui {
	class TextPanel : public Panel
	{
	public:
		std::string name = "TextPanel";
		TextPanel(
			sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, sf::Color backgroundColor, sf::Color edgeColor, int edgeThickness,
			sf::Color textColor, std::string displayText, int fontSize
		);
		virtual void drawSelf(sf::Vector2f relativePosition) override;
		void setText(std::string text);
	private:
		sf::Text text;
	};

}