#include "ProgressBar.h"

gui::ProgressBar::ProgressBar(sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, float value, sf::Color fillColor, sf::Color backgroundColor)
	: Panel(window, position, size, backgroundColor, sf::Color::White, 2, false), m_fillColor(fillColor), m_value(value)
{
}

void gui::ProgressBar::drawSelf(sf::Vector2f relativePosition)
{
	Panel::drawSelf(relativePosition);
	m_quads.clear();

	//std::cout << "Ratio: " << m_value << " -> " << (m_size.x * m_value) << " / " << m_size.x << std::endl;
	sf::Vector2f upperLeft = m_position + relativePosition;
	sf::Vector2f upperRight = sf::Vector2f(upperLeft.x + (m_size.x * m_value), upperLeft.y);
	sf::Vector2f lowerRight = upperLeft + sf::Vector2f(m_size.x*m_value, m_size.y- m_edgeThickness);
	sf::Vector2f lowerLeft = sf::Vector2f(upperLeft.x, upperLeft.y + m_size.y - m_edgeThickness);
	m_quads.append(upperLeft);
	m_quads.append(upperRight);
	m_quads.append(lowerRight);
	m_quads.append(lowerLeft);

	for (int v = 0; v < 4; v++) {
		m_quads[v].color = m_fillColor;
	}
	m_window->draw(m_quads);
}
