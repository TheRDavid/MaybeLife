#include "Slider.h"

gui::Slider::Slider(sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, float value, float max, float min, sf::Color color, action onValueChange)
	: Element(window, position, size, false, true), m_value(value), m_maxValue(max), m_minValue(min), m_color(color), m_onValueChange(onValueChange)
{
	m_name = "Slider";
	m_gripPosition = sf::Vector2f(position.x + ((m_value - m_minValue) / (m_maxValue - m_minValue)) * size.x, -m_gripRadius);
}

void gui::Slider::drawSelf(sf::Vector2f relativePosition)
{
	sf::Vector2f padding = sf::Vector2f(0, 10);
	m_lines.clear();
	sf::Vector2f lineStart = m_position + relativePosition + padding + sf::Vector2f(m_gripRadius, 0);
	sf::Vector2f lineEnd = sf::Vector2f(m_position.x + relativePosition.x + m_size.x - m_gripRadius, m_position.y + relativePosition.y + padding.y);
	float span = lineEnd.x - lineStart.x;
	m_sliderStartPixel = lineStart.x;
	m_sliderEndPixel = lineEnd.x;

	m_lines.append(lineStart);
	m_lines.append(lineEnd);

	m_window->draw(m_lines);

	m_gripPosition = m_position + relativePosition + sf::Vector2f(((m_value - m_minValue) / (m_maxValue - m_minValue)) * span, -m_gripRadius + padding.y);
	sf::CircleShape grip(m_gripRadius);
	grip.setPosition(m_gripPosition);
	grip.setFillColor(m_gripHover ? m_color : sf::Color::Black);
	grip.setOutlineColor(m_color);
	grip.setOutlineThickness(1);
	m_window->draw(grip);
}

void gui::Slider::onMousePressed(sf::Event event)
{
	Element::onMousePressed(event);
	if (m_gripHover)
	{
		m_scrubbing = true;
	}
}

void gui::Slider::onMouseReleased(sf::Event event)
{
	Element::onMouseReleased(event);
	m_scrubbing = false;
}

void gui::Slider::onMouseMove(sf::Event event)
{
	Element::onMouseMove(event);

	bool xGripOverlap = event.mouseMove.x >= (m_gripPosition.x - m_gripRadius * 2) && event.mouseMove.x <= (m_gripPosition.x + m_gripRadius * 2);
	bool yGripOverlap = event.mouseMove.y >= (m_gripPosition.y - m_gripRadius * 2) && event.mouseMove.y <= (m_gripPosition.y + m_gripRadius * 2);

	m_gripHover = xGripOverlap && yGripOverlap;

	if (m_scrubbing)
	{
		// pixels apart from slider start
		float mouseX = std::min((float)event.mouseMove.x, m_sliderEndPixel);
		float newValue = mouseX - m_sliderStartPixel;
		// relative difference in terms of total length of slider (0-1)
		newValue /= m_sliderEndPixel - m_sliderStartPixel;
		// translate into specific value
		newValue *= m_maxValue - m_minValue;
		// consider offset
		newValue += m_minValue;

		newValue = std::min(newValue, m_maxValue);
		newValue = std::max(newValue, m_minValue);

		if (newValue >= m_minValue && newValue <= m_maxValue)
		{
			m_value = newValue;
			m_onValueChange(event);
		}
	}
}

void gui::Slider::onMouseExit(sf::Event event)
{
	Element::onMouseExit(event);
	m_scrubbing = false;
}
