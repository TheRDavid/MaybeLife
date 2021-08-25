#include "Button.h"
#include "GUI.h"

Button::Button(sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, std::string text, std::function<void(sf::Event)> action, sf::Color normalColor)
	: TextPanel(window, position, size, sf::Color::Black, normalColor, 1, normalColor, text, 15), m_action(action)
{
	m_normalColor = normalColor;
	int ox = (int)(m_text.getLocalBounds().width / 2);
	int oy = (int)(m_text.getCharacterSize() / 2);
#pragma warning( suppress : 4244 ) // loss of precision (int to float) is wanted to prevent blurring
	m_text.setOrigin(ox,oy);
	m_text.setPosition(sf::Vector2f(m_size.x / 2, m_size.y / 2));
}

void gui::Button::drawSelf(sf::Vector2f relativePosition)
{
	Panel::drawSelf(relativePosition);
	m_text.setPosition(relativePosition + m_position + sf::Vector2f(m_size.x/2, m_size.y/2 - m_text.getCharacterSize()/4));
	m_window->draw(m_text);
}

void gui::Button::onMouseEnter(sf::Event event)
{
	m_text.setFillColor(sf::Color(255, 255, 200, 255));
	m_backgroundColor = sf::Color(255, 255, 255, 33);
	m_edgeColor = sf::Color(255, 255, 200, 255);
}

void gui::Button::onMouseExit(sf::Event event)
{
	m_text.setFillColor(m_normalColor);
	m_backgroundColor = sf::Color::Black;
	m_edgeColor = m_normalColor;
}

void gui::Button::onMousePressed(sf::Event event)
{
	m_backgroundColor = sf::Color(255, 255, 255, 66);
}

void gui::Button::onMouseReleased(sf::Event event)
{
	m_backgroundColor = sf::Color(255, 255, 255, 33);
	m_action(event);
}
