#include "Element.h"

#include <iostream>

#include "Utilities.h"

Element::Element(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size, bool draggable)
{
	this->m_window = window;
	this->m_draggable = draggable;
	this->m_size = size;
	this->m_position = position;
	m_lastDragPosition = sf::Vector2i(position.x, position.y);
	this->m_drawPosition = position;
	m_triangleStrips = sf::VertexArray(sf::TriangleStrip, 30);
	m_quads = sf::VertexArray(sf::Quads, 20);
	if (m_draggable)
	{
		onMousePressed = &Element::dragStart;
		onMouseReleased = &Element::dragEnd;
	}
}

void Element::handle(sf::Event event)
{
	bool hovering = mouseHovering();
	if (m_draggable)

	if (onMouseMove != nullptr && event.type == event.MouseMoved && hovering)
	{
		(this->*onMouseMove)(event);
	}

	if (onMouseEnter != nullptr && event.type == event.MouseMoved && hovering && !m_mouseHover)
	{
		m_mouseHover = true;
		(this->*onMouseEnter)(event);
	}

	if (onMouseExit != nullptr && event.type == event.MouseMoved && !hovering && m_mouseHover)
	{
		m_mouseHover = false;
		(this->*onMouseExit)(event);
	}

	if (onMousePressed != nullptr && event.type == event.MouseButtonPressed && hovering)
	{
		(this->*onMousePressed)(event);
	}

	if (onMouseReleased != nullptr && event.type == event.MouseButtonReleased && hovering)
	{
		(this->*onMouseReleased)(event);
	}

	if (onKeyPressed != nullptr && event.type == event.KeyPressed && m_focused)
	{
		(this->*onKeyPressed)(event);
	}

	if (onKeyReleased != nullptr && event.type == event.KeyReleased && m_focused)
	{
		(this->*onKeyReleased)(event);
	}
	for (Element* child : m_children)
	{
		child->handle(event);
	}
}

bool Element::mouseHovering()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
	bool xOverlap = mousePos.x > m_drawPosition.x && mousePos.x < m_drawPosition.x + m_size.x;
	bool yOverlap = mousePos.y > m_drawPosition.y && mousePos.y < m_drawPosition.y + m_size.y;
	return xOverlap && yOverlap;
}

void Element::addChild(Element * element)
{
	m_children.push_back(element);
	element->m_parent = this;
}

void Element::drawSelf()
{
	draw(sf::Vector2f(0, 0));
}

void Element::drawChildren(sf::Vector2f relativePosition)
{
	for (Element* child : m_children)
	{
		child->draw(relativePosition + m_position);
	}
}

void Element::dragStart(sf::Event event)
{
	m_dragging = true;
	m_lastDragPosition = sf::Mouse::getPosition(*m_window);
}

void Element::dragEnd(sf::Event event)
{
	m_dragging = false;
}
