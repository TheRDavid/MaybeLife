#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

class Element
{
public:
	std::string m_name = "Element";
	sf::RenderWindow *m_window;
	Element* m_parent;
	std::vector<Element*> m_children;
	sf::Vector2f m_position, m_size, m_drawPosition;
	sf::VertexArray m_triangleStrips, m_quads;

	sf::Vector2i m_lastDragPosition;
	bool m_draggable = false;
	bool m_dragging = false;
	bool m_mouseHover = false;
	bool m_focused = false;

	Element(sf::RenderWindow* m_window, sf::Vector2f position, sf::Vector2f size, bool draggable);

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
		m_drawPosition = relativePosition + m_position;
		drawSelf(relativePosition);
		drawChildren(relativePosition);


		if (m_dragging) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
			sf::Vector2i delta = mousePos - m_lastDragPosition;
			m_position += sf::Vector2f(delta.x, delta.y);
			m_lastDragPosition = mousePos;
		}

	}
	virtual void drawSelf();
	virtual void drawSelf(sf::Vector2f relativePosition) = 0;
	void drawChildren(sf::Vector2f relativePosition);

private:
	void dragStart(sf::Event event);
	void dragEnd(sf::Event event);
};

