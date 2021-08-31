#include "Element.h"

#include <iostream>

#include "Utilities.h"
#include "Commander.h"

Element::Element(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size, bool draggable, bool focusable) : m_focusable(focusable)
{
	this->m_window = window;
	this->m_draggable = draggable;
	this->m_size = size;
	this->m_position = position;
#pragma warning( suppress : 4244 ) // loss of precision (int to float) unavoidable as we're comparing perfect pixel position to world position
	m_lastDragPosition = sf::Vector2i(position.x, position.y);
	this->m_drawPosition = position;
	m_triangleStrips = sf::VertexArray(sf::TriangleStrip, 30);
	m_quads = sf::VertexArray(sf::Quads, 20);
	m_lines = sf::VertexArray(sf::Lines, 20);
}

void Element::handle(sf::Event event)
{
	if (m_enabled)
	{
		bool hovering = mouseHovering();

		if (event.type == event.MouseMoved && hovering)
		{
			(this->onMouseMove)(event);
		}

		if (event.type == event.MouseMoved && hovering && !m_mouseHover)
		{
			if (m_blocksCursor)
			{
				Commander::getInstance().blockCursor(m_id);
			}
			m_mouseHover = true;
			(this->onMouseEnter)(event);
		}

		if (event.type == event.MouseMoved && !hovering && m_mouseHover)
		{
			if (m_blocksCursor)
			{
				Commander::getInstance().unblockCursor(m_id);
			}
			m_mouseHover = false;
			(this->onMouseExit)(event);
		}

		if (event.type == event.MouseButtonPressed && hovering)
		{
			(this->onMousePressed)(event);
		}

		if (event.type == event.MouseButtonReleased && hovering)
		{
			(this->onMouseReleased)(event);
		}

		if (event.type == event.KeyPressed && m_focused)
		{
			(this->onKeyPressed)(event);
		}

		if (event.type == event.KeyReleased && m_focused)
		{
			(this->onKeyReleased)(event);
		}
		for (Element* child : m_children)
		{
			child->handle(event);
		}
	}
}

void Element::onMouseEnter(sf::Event event) {
}
void Element::onMouseExit(sf::Event event)
{
	if (m_focusable)
	{
		Commander::getInstance().deRequestFocus(this);
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
		child->draw(relativePosition + m_position - m_childrenOffset);
	}
}

void Element::onMouseMove(sf::Event event)
{
	if (m_focusable)
	{
		Commander::getInstance().requestFocus(this);
	}
}

void Element::onMousePressed(sf::Event event)
{
	if (m_draggable && !Commander::getInstance().GUIHasFocus())
	{
		m_dragging = true;
		m_lastDragPosition = sf::Mouse::getPosition(*m_window);
	}
}

void Element::onMouseReleased(sf::Event event)
{
	if (m_draggable)
	{
		m_dragging = false;
	}
}
