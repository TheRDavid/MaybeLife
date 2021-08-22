#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Environment.h"
#include "Zone.h"
#include "Commander.h"
class MouseInputManager
{
public:
	MouseInputManager(Environment* environment, sf::RenderWindow* window, sf::View* sceneView, sf::View* uiView);

	void handle(sf::Event event);
private:
	Environment* m_environment;
	sf::RenderWindow* m_window;
	sf::View* m_sceneView, *m_guiView;
	sf::Vector2f m_startDragPos, m_endDragPos;
	bool m_dragging = false;

};
