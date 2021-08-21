#include "InputManager.h"

#include <algorithm>
#include <iterator>
#include <thread>

#include "Utilities.h"
#include "InputMacros.h"

InputManager::InputManager(Environment* environment, sf::RenderWindow* window, sf::View* sceneView, sf::View* guiView)
{
	m_commander = new Commander(environment, window, sceneView, guiView);
	m_mouseInputManager = new MouseInputManager(m_commander, environment, window, sceneView, guiView);
	m_keyboardInputManager = new KeyboardInputManager(m_commander, environment, window, sceneView, guiView);
	m_cliInputManager = new CLIInputManager(m_commander);
}

void InputManager::handleEvents(sf::Event event)
{
	m_keyboardInputManager->handle(event);
	m_mouseInputManager->handle(event);
}
