#include "InputManager.h"

#include <algorithm>
#include <iterator>
#include <thread>

#include "Utilities.h"
#include "InputMacros.h"

InputManager::InputManager(Environment* environment, sf::RenderWindow* window, sf::View* sceneView, sf::View* guiView)
{
	Commander::getInstance().m_environment = environment;
	Commander::getInstance().m_window = window;
	Commander::getInstance().m_sceneView = sceneView;
	Commander::getInstance().m_guiView = guiView;
	m_mouseInputManager = new MouseInputManager(environment, window, sceneView, guiView);
	m_keyboardInputManager = new KeyboardInputManager(environment, window, sceneView, guiView);
	m_cliInputManager = new CLIInputManager();
}

void InputManager::handleEvents(sf::Event event)
{
	m_keyboardInputManager->handle(event);
	m_mouseInputManager->handle(event);
}
