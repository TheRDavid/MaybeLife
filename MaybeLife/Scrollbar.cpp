#include "Scrollbar.h"

gui::Scrollbar::Scrollbar(sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, float min, float max, float value)
	: Panel(window, position, size, sf::Color(40, 40, 40, 100),
		sf::Color::White, 1, false), m_min(min), m_max(max), m_value(value)
{
	m_name = "ScrollBar";
	m_handleColor = sf::Color(255, 255, 255, 200);
	m_blocksCursor = true;
	m_focusable = true;
}

void gui::Scrollbar::drawSelf(sf::Vector2f relativePosition)
{
	sf::Vector2f relPosition = relativePosition + m_position;
	sf::RectangleShape bgRect;
	bgRect.setSize(m_size);
	bgRect.setFillColor(m_backgroundColor);
	bgRect.setOutlineColor(m_edgeColor);
	bgRect.setOutlineThickness(1);
	bgRect.setPosition(relPosition);

	float valueSpan = m_max - m_min;
	float yPos = relPosition.y + m_value / valueSpan * m_size.y;

	m_handleRect.setSize(sf::Vector2f(m_size.x, handleLength * m_size.y));
	m_barStartPixel = bgRect.getPosition().y;
	m_barEndPixel = bgRect.getPosition().y + bgRect.getSize().y;
	if (m_handleHover)
	{
		m_handleRect.setFillColor(m_handleColor);
	}
	else {
		m_handleRect.setFillColor(sf::Color(150, 150, 150, 150));
	}
	m_handleRect.setOutlineColor(m_edgeColor);
	m_handleRect.setOutlineThickness(1);
	m_handleRect.setPosition(sf::Vector2f(relPosition.x, yPos));

	m_window->draw(bgRect);
	m_window->draw(m_handleRect);
}

void gui::Scrollbar::onMousePressed(sf::Event event)
{
	Panel::onMousePressed(event);
	if (m_handleHover)
	{
		m_handleDragOffset = lastCursorPosition.y - m_handleRect.getPosition().y;
		m_handleMove = true;
	}
}

void gui::Scrollbar::onMouseReleased(sf::Event event)
{
	Panel::onMouseReleased(event);

	m_handleMove = false;
}

void gui::Scrollbar::onMouseMove(sf::Event event)
{
	lastCursorPosition = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
	Panel::onMouseMove(event);
	// Somehow wrong!
	bool yMin = event.mouseMove.y >= m_handleRect.getPosition().y;
	bool yMax = event.mouseMove.y <= m_handleRect.getPosition().y + m_handleRect.getSize().y;
	if (yMin && yMax)
	{
		m_handleHover = true;
	}
	else {
		m_handleHover = false;
	}

	if (m_handleMove)
	{
		// pixels apart from slider start
		float mouseY = std::min((float)event.mouseMove.y, m_barEndPixel);
		float newValue = mouseY - m_barStartPixel;
		// relative difference in terms of total length of slider (0-1)
		newValue /= m_barEndPixel - m_barStartPixel;
		// translate into specific value
		newValue *= m_max - m_min;
		// consider offset
		newValue += m_min;
		// consider mouse position on handle bar
		float valueOffset = m_handleDragOffset / (m_barEndPixel - m_barStartPixel) * (m_max - m_min);

		newValue = std::min(newValue, m_max);
		newValue = std::max(newValue, m_min);

		if (newValue >= m_min && newValue <= m_max)
		{
			m_value = newValue;
			//m_onValueChange(event);
		}
	}

}

void gui::Scrollbar::onMouseExit(sf::Event event)
{
	Panel::onMouseExit(event);
	m_handleHover = false;
	m_handleMove = false;
}
