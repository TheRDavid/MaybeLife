#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>

#include "Utilities.h"

class Element
{
public:
	unsigned int m_id = ID();
	std::string m_name = "Element";
	sf::RenderWindow *m_window;
	Element* m_parent;
	std::vector<Element*> m_children;
	sf::Vector2f m_position, m_size, m_drawPosition;
	sf::VertexArray m_triangleStrips, m_quads, m_lines;

	sf::Vector2i m_lastDragPosition;
	bool m_draggable = false;
	bool m_dragging = false;
	bool m_mouseHover = false;
	bool m_focused = false;
	bool m_blocksCursor = true;
	bool m_focusable = false;

	Element(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size, bool draggable, bool focusable);

	void handle(sf::Event event);

	virtual void onMouseEnter(sf::Event event);
	virtual void onMouseExit(sf::Event event);
	virtual void onMouseMove(sf::Event event);
	virtual void onMousePressed(sf::Event event);
	virtual void onMouseReleased(sf::Event event);
	virtual void onKeyPressed(sf::Event event) {};
	virtual void onKeyReleased(sf::Event event) {};

	bool mouseHovering();

	void addChild(Element* element);

	void draw() {
		draw(sf::Vector2f(0, 0));
	}
	void draw(sf::Vector2f relativePosition) {
		m_drawPosition = relativePosition + m_position;
		drawSelf(relativePosition);
		drawChildren(relativePosition);


		if (m_dragging) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
			sf::Vector2i delta = mousePos - m_lastDragPosition;
#pragma warning( suppress : 4244 ) // loss of precision (Vector2i to Vector2f) not an issue
			m_position += sf::Vector2f(delta.x, delta.y);
			m_lastDragPosition = mousePos;
		}

	}
	virtual void drawSelf();
	virtual void drawSelf(sf::Vector2f relativePosition) = 0;
	void drawChildren(sf::Vector2f relativePosition);

private:
	static unsigned int ID()
	{
		static unsigned int ID = 0;
		return ID++;
	}
};

