#include "TextPanel.h"

#include <iostream>

#include "Utilities.h"

TextPanel::TextPanel(sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, sf::Color backgroundColor, sf::Color edgeColor, int edgeThickness, sf::Color textColor, std::string displayText, int fontSize)
	: Panel(window, position, size, backgroundColor, edgeColor, edgeThickness, false)
{
	m_text = sf::Text(displayText, AppConfig::getInstance().m_mainFont, fontSize);
	m_text.setFillColor(textColor);
}

void TextPanel::drawSelf(sf::Vector2f relativePosition)
{
	Panel::drawSelf(relativePosition);
	m_text.setPosition(relativePosition + m_position - sf::Vector2f(m_childrenOffset.x * m_text.getLocalBounds().width, m_childrenOffset.y * m_text.getLocalBounds().height));
	m_window->draw(m_text);
}

void gui::TextPanel::setText(std::string displayText)
{
	m_text.setString(displayText);
}
