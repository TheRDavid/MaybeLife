#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

class Element
{
public:
	std::string name = "Element";
	sf::RenderWindow *window;
	Element* parent;
	std::vector<Element*> children;
	sf::Vector2f position, size, drawPosition;
	sf::VertexArray triangleStrips, quads;

	sf::Vector2i lastDragPosition;
	bool draggable = false;
	bool dragging = false;
	bool mouseHover = false;
	bool focused = false;

	Element(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size, bool draggable);

	void handle(sf::Event event);

	void(Element::*onMouseMove)(sf::Event) = nullptr;
	void(Element::*onMouseEnter)(sf::Event) = nullptr;
	void(Element::*onMouseExit)(sf::Event) = nullptr;
	void(Element::*onMousePressed)(sf::Event) = nullptr;
	void(Element::*onMouseReleased)(sf::Event) = nullptr;
	void(Element::*onKeyPressed)(sf::Event) = nullptr;
	void(Element::*onKeyReleased)(sf::Event) = nullptr;

	bool mouseHovering();

	void addChild(Element* element);

	void draw() {
		draw(sf::Vector2f(0, 0));
	}
	void draw(sf::Vector2f relativePosition) {
		drawPosition = relativePosition + position;
		drawSelf(relativePosition);
		drawChildren(relativePosition);


		if (dragging) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
			sf::Vector2i delta = mousePos - lastDragPosition;
			position += sf::Vector2f(delta.x, delta.y);
			lastDragPosition = mousePos;
		}

	}
	virtual void drawSelf();
	virtual void drawSelf(sf::Vector2f relativePosition) = 0;
	void drawChildren(sf::Vector2f relativePosition);

private:
	void dragStart(sf::Event event);
	void dragEnd(sf::Event event);
};

