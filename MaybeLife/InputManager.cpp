#include "InputManager.h"

#include <algorithm>
#include <iterator>
#include <thread>

#include "Utilities.h"
#include "InputMacros.h"

InputManager::InputManager(Environment* environment, sf::RenderWindow* window, sf::View* sceneView, sf::View* uiView)
{
	commander = new Commander(environment, window, sceneView, uiView);
	mouseInputManager = new MouseInputManager(commander, environment, window, sceneView, uiView);
	keyboardInputManager = new KeyboardInputManager(commander, environment, window, sceneView, uiView);
	cliInputManager = new CLIInputManager(commander);
}

void InputManager::handleEvents(sf::Event event)
{
	keyboardInputManager->handle(event);
	mouseInputManager->handle(event);
}
