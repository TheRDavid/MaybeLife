#include "Element.h"

#include <iostream>

#include "Utilities.h"

Element::Element(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size, bool draggable)
{
	this->window = window;
	this->parent = parent;
	this->draggable = draggable;
	this->size = size;
	this->position = position;
	lastDragPosition = sf::Vector2i(position.x, position.y);
	this->drawPosition = position;
	triangleStrips = sf::VertexArray(sf::TriangleStrip, 30);
	quads = sf::VertexArray(sf::Quads, 20);
	if (draggable)
	{
		onMousePressed = &Element::dragStart;
		onMouseReleased = &Element::dragEnd;
	}
}

void Element::handle(sf::Event event)
{
	bool hovering = mouseHovering();
	if (draggable)

	if (onMouseMove != nullptr && event.type == event.MouseMoved && hovering)
	{
		(this->*onMouseMove)(event);
	}

	if (onMouseEnter != nullptr && event.type == event.MouseMoved && hovering && !mouseHover)
	{
		mouseHover = true;
		(this->*onMouseEnter)(event);
	}

	if (onMouseExit != nullptr && event.type == event.MouseMoved && !hovering && mouseHover)
	{
		mouseHover = false;
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

	if (onKeyPressed != nullptr && event.type == event.KeyPressed && focused)
	{
		(this->*onKeyPressed)(event);
	}

	if (onKeyReleased != nullptr && event.type == event.KeyReleased && focused)
	{
		(this->*onKeyReleased)(event);
	}
	for (Element* child : children)
	{
		child->handle(event);
	}
}

bool Element::mouseHovering()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	bool xOverlap = mousePos.x > drawPosition.x && mousePos.x < drawPosition.x + size.x;
	bool yOverlap = mousePos.y > drawPosition.y && mousePos.y < drawPosition.y + size.y;
	return xOverlap && yOverlap;
}

void Element::addChild(Element * element)
{
	children.push_back(element);
	element->parent = this;
}

void Element::drawSelf()
{
	draw(sf::Vector2f(0, 0));
}

void Element::drawChildren(sf::Vector2f relativePosition)
{
	for (Element* child : children)
	{
		child->draw(relativePosition + position);
	}
}

void Element::dragStart(sf::Event event)
{
	dragging = true;
	lastDragPosition = sf::Mouse::getPosition(*window);
}

void Element::dragEnd(sf::Event event)
{
	dragging = false;
}
