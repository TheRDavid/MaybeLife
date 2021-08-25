#include "Panel.h"

#include <iostream>

#include "Utilities.h"

using namespace gui;

Panel::Panel(sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, sf::Color backgroundColor, sf::Color edgeColor, int edgeThickness, bool draggable)
	: Element(window, position, size, draggable)
{
	this->m_backgroundColor = backgroundColor;
	this->m_edgeColor = edgeColor;
	this->m_edgeThickness = edgeThickness;
}

void Panel::drawSelf(sf::Vector2f relativePosition)
{
	m_quads.clear();
	m_triangleStrips.clear();

	sf::Vector2f thicknessVec = sf::Vector2f((float)m_edgeThickness, (float)m_edgeThickness);
	sf::Vector2f upperLeft = m_position + relativePosition;
	sf::Vector2f upperRight = sf::Vector2f(upperLeft.x + m_size.x, upperLeft.y);
	sf::Vector2f lowerRight = upperLeft + m_size;
	sf::Vector2f lowerLeft = sf::Vector2f(upperLeft.x, upperLeft.y + m_size.y);

	m_triangleStrips.append(sf::Vector2f(upperLeft.x - m_edgeThickness, upperLeft.y-m_edgeThickness));
	m_triangleStrips.append(sf::Vector2f(upperLeft.x, upperLeft.y));
	m_triangleStrips.append(sf::Vector2f(upperRight.x, upperRight.y - m_edgeThickness));
	m_triangleStrips.append(sf::Vector2f(upperRight.x - m_edgeThickness , upperRight.y));
	m_triangleStrips.append(sf::Vector2f(lowerRight.x, lowerRight.y));
	m_triangleStrips.append(sf::Vector2f(lowerRight.x - m_edgeThickness, lowerRight.y-m_edgeThickness));
	m_triangleStrips.append(lowerLeft);
	m_triangleStrips.append(lowerLeft- thicknessVec);
	m_triangleStrips.append(sf::Vector2f(upperLeft.x, upperLeft.y));
	m_triangleStrips.append(sf::Vector2f(upperLeft.x - m_edgeThickness, upperLeft.y - m_edgeThickness));

	for (int v = 0; v < m_triangleStrips.getVertexCount(); v++) {
		m_triangleStrips[v].color = m_edgeColor;
	}

	m_quads.append(upperLeft);
	m_quads.append(upperRight);
	m_quads.append(lowerRight);
	m_quads.append(lowerLeft);

	for (int v = 0; v < m_quads.getVertexCount(); v++) {
		m_quads[v].color = m_backgroundColor;
	}

	m_window->draw(m_quads);
	m_window->draw(m_triangleStrips);
}
