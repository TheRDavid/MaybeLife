#pragma once
#include <SFML/Config.hpp>

#include "MouseInputManager.h"
#include "KeyboardInputManager.h"
#include "CLIInputManager.h"
class InputManager
{
public:
	Commander* m_commander;

	InputManager(Environment* environment, sf::RenderWindow* m_window, sf::View* sceneView, sf::View* m_guiView);

	void handleEvents(sf::Event event);
private:
	MouseInputManager* m_mouseInputManager;
	KeyboardInputManager* m_keyboardInputManager;
	CLIInputManager* m_cliInputManager;
};

