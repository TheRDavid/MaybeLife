#include "KeyboardInputManager.h"

#include <iostream>

#include "Utilities.h"

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
			window->setView(*sceneView);
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
			sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);

			float relX = 1 - (sceneView->getCenter().x + sceneView->getSize().x / 2 - worldPos.x) / sceneView->getSize().x;
			float relY = 1 - (sceneView->getCenter().y + sceneView->getSize().y / 2 - worldPos.y) / sceneView->getSize().y;
			sf::Vector2f relativePos = sf::Vector2f(relX, relY);
			sceneView->zoom(0.9);

			pixelPos = sf::Mouse::getPosition(*window);
			worldPos = window->mapPixelToCoords(pixelPos);

			float relX2 = 1 - (sceneView->getCenter().x + sceneView->getSize().x / 2 - worldPos.x) / sceneView->getSize().x;
			float relY2 = 1 - (sceneView->getCenter().y + sceneView->getSize().y / 2 - worldPos.y) / sceneView->getSize().y;
			sf::Vector2f relativePos2 = sf::Vector2f(relX2, relY2);

			sf::Vector2f relDelta = (relativePos2 - relativePos);
			sceneView->setCenter(sceneView->getCenter() + sf::Vector2f(relDelta.x * sceneView->getSize().x, relDelta.y * sceneView->getSize().y));
			environment->adjustRenderingRect();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
			window->setView(*sceneView);
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
			sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);

			float relX = 1 - (sceneView->getCenter().x + sceneView->getSize().x / 2 - worldPos.x) / sceneView->getSize().x;
			float relY = 1 - (sceneView->getCenter().y + sceneView->getSize().y / 2 - worldPos.y) / sceneView->getSize().y;
			sf::Vector2f relativePos = sf::Vector2f(relX, relY);
			sceneView->zoom(1.1);
			pixelPos = sf::Mouse::getPosition(*window);
			worldPos = window->mapPixelToCoords(pixelPos);

			float relX2 = 1 - (sceneView->getCenter().x + sceneView->getSize().x / 2 - worldPos.x) / sceneView->getSize().x;
			float relY2 = 1 - (sceneView->getCenter().y + sceneView->getSize().y / 2 - worldPos.y) / sceneView->getSize().y;
			sf::Vector2f relativePos2 = sf::Vector2f(relX2, relY2);

			sf::Vector2f relDelta = (relativePos2 - relativePos);
			sceneView->setCenter(sceneView->getCenter() + sf::Vector2f(relDelta.x * sceneView->getSize().x, relDelta.y * sceneView->getSize().y));
			environment->adjustRenderingRect();
		}
	}
}