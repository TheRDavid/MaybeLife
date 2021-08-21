#include "TextPanel.h"

#include <iostream>

#include "Utilities.h"

TextPanel::TextPanel(sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, sf::Color backgroundColor, sf::Color edgeColor, int edgeThickness, sf::Color textColor, std::string displayText, int fontSize)
	: Panel(window, position, size, backgroundColor, edgeColor, edgeThickness, false)
{
	text = sf::Text(displayText, AppConfig::getInstance().mainFont, fontSize);
	text.setFillColor(textColor);
}

void TextPanel::drawSelf(sf::Vector2f relativePosition)
{
	Panel::drawSelf(relativePosition);
	text.setPosition(relativePosition + position);
	window->draw(text);
}

void gui::TextPanel::setText(std::string displayText)
{
	text.setString(displayText);
}
