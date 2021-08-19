#include "InputManager.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <thread>

#include "Environment.h"
#include "Utilities.h"
#include "InputMacros.h"

InputManager::InputManager(Environment* environment, sf::RenderWindow* window, sf::View* sceneView, sf::View* uiView)
{
	this->environment = environment;
	this->window = window;
	new std::thread(&InputManager::catchInput, this);
	this->sceneView = sceneView;
	this->uiView = uiView;
	zone = environment->zoneAt(sf::Vector2f(0, 0));
}

void InputManager::setBehaviour(std::string behaviour)
{
	Entity::Behaviour newBehaviour;
	if (behaviour == "gravitate") {
		newBehaviour = Entity::Behaviour::GRAVITATE;
	}
	else if (behaviour == "random") {
		newBehaviour = Entity::Behaviour::RANDOM;
	}
	else if (behaviour == "sleep") {
		newBehaviour = Entity::Behaviour::SLEEP;
	}
	else if (behaviour == "spread") {
		newBehaviour = Entity::Behaviour::SPREAD;
	}
	else if (behaviour == "fall") {
		newBehaviour = Entity::Behaviour::FALL;
	}
	else if (behaviour == "group") {
		newBehaviour = Entity::Behaviour::GROUP;
	}
	else {
		std::cout << "ERROR: Invalid Behaviour" << std::endl;
		newBehaviour = Entity::Behaviour::RANDOM;
	}
	for (Entity* entity : *(environment->entities)) {
		entity->behaviour = newBehaviour;
	}
}

void InputManager::setGravityCenter(std::string x, std::string y)
{
	std::string errorMsg = "ERROR: Invalid Gravity Center";
	try {
		int centerX = std::stoi(x);
		int centerY = std::stoi(y);
		environment->gravityCenter = sf::Vector2f(centerX, centerY);
		return;
	}
	catch (const std::invalid_argument&) {
		std::cout << errorMsg << std::endl;
	}
	std::cout << errorMsg << std::endl;
}

void InputManager::handleEvents()
{
	std::string titleString = environment->stepsToString();
	sf::Event event;
	if (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();

		handleKeyboardCommands(event);

		if (event.type == sf::Event::MouseMoved) {
			window->setView(*sceneView);
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);

			// convert it to world coordinates
			sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
			zone = environment->zoneAt(worldPos);

			if (dragging) {
				window->setView(*uiView);
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);

				// convert it to world coordinates
				// BROKEN ONCE SOOMED IN?!?!?!
				sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
				endDragPos = worldPos;
				sf::Vector2f delta = sf::Vector2f((startDragPos.x - endDragPos.x) * currentZoom, (startDragPos.y - endDragPos.y) * currentZoom);
				std::cout << "In drag, delta = " << ut::to_string(delta) << " at zoom " << currentZoom << std::endl;
				sceneView->move(delta);
				startDragPos = endDragPos;
				std::cout << "VP: center=" << ut::to_string(sceneView->getCenter()) << ", rect=" << ut::to_string(sceneView->getSize()) << std::endl;
				environment->renderRectPosition = sceneView->getCenter() - sf::Vector2f(sceneView->getSize().x / 2, sceneView->getSize().y / 2);
				environment->renderRectSize = sf::Vector2f(sceneView->getSize().x, sceneView->getSize().y);
			}

		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Button::Right) {
				window->setView(*uiView);
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
				startDragPos = window->mapPixelToCoords(pixelPos);
				std::cout << "Mouse down at: " << ut::to_string(startDragPos) << std::endl;
				dragging = true;
			}
			else if (event.mouseButton.button == sf::Mouse::Button::Middle) {
				if (zone != nullptr) {
					window->setView(*sceneView);
					sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
					sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
					Entity* entity = new Entity(environment, Entity::Behaviour::RANDOM, worldPos);
					Zone* zone = environment->zoneAt(worldPos);
					zone->addEntity(entity);
					std::cout << "Adding entity" << std::endl << entity->to_string() << std::endl << "to zone" << std::endl << zone->toString() << std::endl;
					environment->insertLock.lock();
					environment->entities->push_back(entity);
					environment->insertLock.unlock();
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Button::Left) {
				window->setView(*sceneView);
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
				sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
				environment->selectedZone = environment->zoneAt(worldPos);
				if (environment->selectedZone != nullptr) {
					std::cout << "Setting Selected Zone: " << std::endl << environment->selectedZone->toString() << std::endl;
				}
			}
		}
		if (event.type == sf::Event::MouseButtonReleased) {
			dragging = false;

		}
		std::cout << "Dragging: " << dragging << " from " << ut::to_string(startDragPos) << std::endl;
	}
	if (zone == nullptr) {
		titleString += " Outside World";
	}
	else
		titleString += " " + zone->toString();
	window->setTitle(titleString);
}

void InputManager::catchInput()
{
	while (true) {
		std::cout << ":";
		std::string cmd;
		std::getline(std::cin, cmd);
		std::istringstream iss(cmd);
		std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
					  std::istream_iterator<std::string>{} };
		execute(tokens);
	}
}

void InputManager::handleKeyboardCommands(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
			sceneView->zoom(0.9);
			environment->renderRectPosition = sceneView->getCenter() - sf::Vector2f(sceneView->getSize().x / 2, sceneView->getSize().y / 2);
			environment->renderRectSize = sf::Vector2f(sceneView->getSize().x, sceneView->getSize().y);
			currentZoom = (sceneView->getSize().x / uiView->getSize().x + sceneView->getSize().y / uiView->getSize().y) / 2;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
			sceneView->zoom(1.1);
			environment->renderRectPosition = sceneView->getCenter() - sf::Vector2f(sceneView->getSize().x / 2, sceneView->getSize().y / 2);
			environment->renderRectSize = sf::Vector2f(sceneView->getSize().x, sceneView->getSize().y);
			currentZoom = (sceneView->getSize().x / uiView->getSize().x + sceneView->getSize().y / uiView->getSize().y) / 2;
		}
	}
}

void InputManager::execute(std::vector<std::string> tokens)
{
	if (tokens.size() == 0) return;
	std::string command = tokens[0];
	switch (resolveCommand(command)) {

	case set_behaviour:
		if (tokens.size() != 2)
		{
			std::cout << "ERROR: Invalid Number of Arguments" << std::endl;
			break;
		}
		setBehaviour(tokens[1]); break;

	case set_collide:
		if (tokens.size() != 2)
		{
			std::cout << "ERROR: Invalid Number of Arguments" << std::endl;
			break;
		}
		environment->entityCollision = tokens[1] == "true"; break;

	case show_zones:
		if (tokens.size() != 2)
		{
			std::cout << "ERROR: Invalid Number of Arguments" << std::endl;
			break;
		}
		environment->showZones = tokens[1] == "true"; break;

	case show_ui:
		if (tokens.size() != 2)
		{
			std::cout << "ERROR: Invalid Number of Arguments" << std::endl;
			break;
		}
		environment->showUI = tokens[1] == "true"; break;

	case show_lines:
		if (tokens.size() != 2)
		{
			std::cout << "ERROR: Invalid Number of Arguments" << std::endl;
			break;
		}
		environment->showLines = tokens[1] == "true"; break;

	case set_gravity_center:
		if (tokens.size() != 3)
		{
			std::cout << "ERROR: Invalid Number of Arguments" << std::endl;
			break;
		}
		setGravityCenter(tokens[1], tokens[2]); break;

	default:
		std::cout << "ERROR: Invalid Command" << std::endl;
	}
}
