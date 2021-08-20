#include "KeyboardInputManager.h"


KeyboardInputManager::KeyboardInputManager(Commander * commander, Environment * environment, sf::RenderWindow * window, sf::View * sceneView, sf::View * uiView)
{
	this->commander = commander;
	this->environment = environment;
	this->window = window;
	this->sceneView = sceneView;
	this->uiView = uiView;
}

void KeyboardInputManager::handle(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
			sceneView->zoom(0.9);
			environment->renderRectPosition = sceneView->getCenter() - sf::Vector2f(sceneView->getSize().x / 2, sceneView->getSize().y / 2);
			environment->renderRectSize = sf::Vector2f(sceneView->getSize().x, sceneView->getSize().y);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
			sceneView->zoom(1.1);
			environment->renderRectPosition = sceneView->getCenter() - sf::Vector2f(sceneView->getSize().x / 2, sceneView->getSize().y / 2);
			environment->renderRectSize = sf::Vector2f(sceneView->getSize().x, sceneView->getSize().y);
		}
	}
}