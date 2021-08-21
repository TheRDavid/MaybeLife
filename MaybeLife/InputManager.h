#pragma once
#include <SFML/Config.hpp>

#include "MouseInputManager.h"
#include "KeyboardInputManager.h"
#include "CLIInputManager.h"
class InputManager
{
public:
	Commander* commander;

	InputManager(Environment* environment, sf::RenderWindow* window, sf::View* sceneView, sf::View* uiView);

	void handleEvents(sf::Event event);
private:
	MouseInputManager* mouseInputManager;
	KeyboardInputManager* keyboardInputManager;
	CLIInputManager* cliInputManager;
};

