#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Environment.h"
#include "Zone.h"
#include "Commander.h"
class MouseInputManager
{
public:
	MouseInputManager(Commander* commander, Environment* environment, sf::RenderWindow* window, sf::View* sceneView, sf::View* uiView);

	void handle(sf::Event event);
private:
	Commander* commander;
	Environment* environment;
	sf::RenderWindow* window;
	sf::View* sceneView, *uiView;
	sf::Vector2f startDragPos, endDragPos;
	bool dragging = false;

};

