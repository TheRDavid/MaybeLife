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
	sf::Vector2i m_mousePixelPos;
	sf::Vector2f m_mouseWorldPos;
	sf::Vector2f m_arealSelectionStart, m_arealSelectionStop;

	bool m_makingArealSelection = false, m_activeArealSelection = false;
private:
	Environment* m_environment;
	sf::RenderWindow* m_window;
	sf::View* m_sceneView, *m_guiView;
	sf::Vector2f m_startDragPos, m_endDragPos;
	bool m_dragging = false;

};
