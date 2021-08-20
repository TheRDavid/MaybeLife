#include "MouseInputManager.h"

#include "Utilities.h"
#include "Grid.h"

MouseInputManager::MouseInputManager(Commander * commander, Environment * environment, sf::RenderWindow * window, sf::View * sceneView, sf::View * uiView)
{
	this->commander = commander;
	this->environment = environment;
	this->window = window;
	this->sceneView = sceneView;
	this->uiView = uiView;
}

void MouseInputManager::handle(sf::Event event)
{
	if (event.type == sf::Event::Closed)
		window->close();

	if (event.type == sf::Event::MouseMoved) {
		window->setView(*sceneView);
		sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);

		// convert it to world coordinates
		sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
		commander->setSelectedZone(environment->entityGrid->zoneAt(worldPos));

		if (dragging) {
			window->setView(*uiView);
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);

			sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
			endDragPos = worldPos;
			float currentZoom = (sceneView->getSize().x / environment->size.x + sceneView->getSize().y / environment->size.y) / 2;
			sf::Vector2f delta = sf::Vector2f((startDragPos.x - endDragPos.x) * currentZoom, (startDragPos.y - endDragPos.y) * currentZoom);
			//std::cout << "In drag, delta = " << ut::to_string(delta) << " at zoom " << currentZoom << std::endl;
			sceneView->move(delta);
			startDragPos = endDragPos;
			//std::cout << "VP: center=" << ut::to_string(sceneView->getCenter()) << ", rect=" << ut::to_string(sceneView->getSize()) << std::endl;
			environment->renderRectPosition = sceneView->getCenter() - sf::Vector2f(sceneView->getSize().x / 2, sceneView->getSize().y / 2);
			environment->renderRectSize = sf::Vector2f(sceneView->getSize().x, sceneView->getSize().y);
		}

	}

	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Button::Right) {
			window->setView(*uiView);
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
			startDragPos = window->mapPixelToCoords(pixelPos);
			//std::cout << "Mouse down at: " << ut::to_string(startDragPos) << std::endl;
			dragging = true;
		}
		else if (event.mouseButton.button == sf::Mouse::Button::Middle) {
			window->setView(*sceneView);
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
			sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
			commander->addEntity(worldPos);
		}
		else if (event.mouseButton.button == sf::Mouse::Button::Left) {
			window->setView(*sceneView);
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
			sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
			commander->selectZoneAt(worldPos);
		}
	}
	if (event.type == sf::Event::MouseButtonReleased) {
		dragging = false;

	}
	//std::cout << "Dragging: " << dragging << " from " << ut::to_string(startDragPos) << std::endl;

}
