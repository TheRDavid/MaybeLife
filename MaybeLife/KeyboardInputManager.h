#pragma once
#include <SFML/Graphics.hpp>

#include "Environment.h"
#include "Zone.h"
#include "Commander.h"
class KeyboardInputManager
{
public:
	KeyboardInputManager(Commander* commander, Environment* environment, sf::RenderWindow* window, sf::View* sceneView, sf::View* uiView);

	void handle(sf::Event event);

private:
	Commander* commander;
	Environment* environment;
	sf::RenderWindow* window;
	sf::View* sceneView, *uiView;
};

