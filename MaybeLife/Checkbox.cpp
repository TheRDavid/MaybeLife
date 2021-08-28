#include "Checkbox.h"

#include "Commander.h"

gui::Checkbox::Checkbox(sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, sf::Color edgeColor, sf::Color textColor, std::string displayText, int fontSize, bool checked)
	: TextPanel(window, position, size, sf::Color::Transparent, edgeColor, 0, textColor, displayText, fontSize), m_checked(checked)
{
}

void gui::Checkbox::drawSelf(sf::Vector2f relativePosition)
{
	// override drawing done by textpanel
	Panel::drawSelf(relativePosition);
	m_text.setPosition(relativePosition + m_position + sf::Vector2f(30, 0));
	m_lines.clear();
	int idx = 0;
	m_lines.append(sf::Vector2f(relativePosition + m_position));
	m_lines[idx++].color = m_edgeColor;
	m_lines.append(sf::Vector2f(relativePosition + m_position + sf::Vector2f(20, 0)));
	m_lines[idx++].color = m_edgeColor;
	m_lines.append(sf::Vector2f(relativePosition + m_position + sf::Vector2f(20, 0)));
	m_lines[idx++].color = m_edgeColor;
	m_lines.append(sf::Vector2f(relativePosition + m_position + sf::Vector2f(20, 20)));
	m_lines[idx++].color = m_edgeColor;
	m_lines.append(sf::Vector2f(relativePosition + m_position + sf::Vector2f(20, 20)));
	m_lines[idx++].color = m_edgeColor;
	m_lines.append(sf::Vector2f(relativePosition + m_position + sf::Vector2f(0, 20)));
	m_lines[idx++].color = m_edgeColor;
	m_lines.append(sf::Vector2f(relativePosition + m_position + sf::Vector2f(0, 20)));
	m_lines[idx++].color = m_edgeColor;
	m_lines.append(sf::Vector2f(relativePosition + m_position));
	m_lines[idx++].color = m_edgeColor;

	if (m_checked)
	{
		m_lines.append(sf::Vector2f(relativePosition + m_position + sf::Vector2f(4, 10)));
		m_lines[idx++].color = m_edgeColor;
		m_lines.append(sf::Vector2f(relativePosition + m_position + sf::Vector2f(10, 16)));
		m_lines[idx++].color = m_edgeColor;
		m_lines.append(sf::Vector2f(relativePosition + m_position + sf::Vector2f(10, 16)));
		m_lines[idx++].color = m_edgeColor;
		m_lines.append(sf::Vector2f(relativePosition + m_position + sf::Vector2f(16, 4)));
		m_lines[idx++].color = m_edgeColor;
	}

	m_window->draw(m_text);
	m_window->draw(m_lines);
}

void gui::Checkbox::onMouseReleased(sf::Event event)
{
	m_checked = !m_checked;
	if (m_checked)
	{
		Commander::getInstance().record();
	}
	else {
		Commander::getInstance().stopRecording();
	}
}
