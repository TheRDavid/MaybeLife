#pragma once
#include <SFML/Graphics.hpp>

#include "Environment.h"
#include "Zone.h"
#include "Commander.h"
class KeyboardInputManager
{
public:
	KeyboardInputManager(Environment* environment, sf::RenderWindow* window, sf::View* sceneView, sf::View* guiView);

	void handle(sf::Event event);

private:
	Environment* m_environment;
	sf::RenderWindow* m_window;
	sf::View* m_sceneView, *m_guiView;
};
