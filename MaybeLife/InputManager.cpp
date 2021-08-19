#include "InputManager.h"
#include "InputMacros.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <thread>
#include "Utilities.h"

InputManager::InputManager(Environment* environment, RenderWindow* window, sf::View* sceneView, sf::View* uiView)
{
	this->environment = environment;
	this->window = window;
	new thread(&InputManager::catchInput, this);
	this->sceneView = sceneView;
	this->uiView = uiView;
	zone = environment->zoneAt(Vector2f(0, 0));
}

void InputManager::setBehaviour(string behaviour)
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
		cout << "ERROR: Invalid Behaviour" << endl;
		newBehaviour = Entity::Behaviour::RANDOM;
	}
	for (Entity* entity : *(environment->entities)) {
		entity->behaviour = newBehaviour;
	}
}

void InputManager::setGravityCenter(string x, string y)
{
	string errorMsg = "ERROR: Invalid Gravity Center";
	try {
		int centerX = stoi(x);
		int centerY = stoi(y);

		environment->gravityCenter = Vector2f(centerX, centerY);
		return;
	}
	catch (const std::invalid_argument&) {
		cout << errorMsg << endl;
	}
	cout << errorMsg << endl;
}

void InputManager::handleEvents()
{
	string titleString = environment->stepsToString();
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
				Vector2f delta = Vector2f((startDragPos.x - endDragPos.x) * currentZoom, (startDragPos.y - endDragPos.y) * currentZoom);
				cout << "In drag, delta = " << ut::to_string(delta) << " at zoom " << currentZoom << endl;
				sceneView->move(delta);
				startDragPos = endDragPos;
				cout << "VP: center=" << ut::to_string(sceneView->getCenter()) << ", rect=" << ut::to_string(sceneView->getSize()) << endl;
				environment->renderRectPosition = sceneView->getCenter() - Vector2f(sceneView->getSize().x / 2, sceneView->getSize().y / 2);
				environment->renderRectSize = Vector2f(sceneView->getSize().x, sceneView->getSize().y);
			}

		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Button::Right) {
				window->setView(*uiView);
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
				startDragPos = window->mapPixelToCoords(pixelPos);
				cout << "Mouse down at: " << ut::to_string(startDragPos) << endl;
				dragging = true;
			}
			else if (event.mouseButton.button == sf::Mouse::Button::Middle) {
				if (zone != nullptr) {
					window->setView(*sceneView);
					sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
					Vector2f worldPos = window->mapPixelToCoords(pixelPos);
					Entity* entity = new Entity(environment, Entity::Behaviour::RANDOM, worldPos);
					Zone* zone = environment->zoneAt(worldPos);
					zone->addEntity(entity);
					cout << "Adding entity" << endl << entity->to_string() << endl << "to zone" << endl << zone->toString() << endl;
					environment->insertLock.lock();
					environment->entities->push_back(entity);
					environment->insertLock.unlock();
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Button::Left) {
				window->setView(*sceneView);
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
				Vector2f worldPos = window->mapPixelToCoords(pixelPos);
				environment->selectedZone = environment->zoneAt(worldPos);
				if (environment->selectedZone != nullptr) {
					cout << "Setting Selected Zone: " << endl << environment->selectedZone->toString() << endl;
				}
			}
		}
		if (event.type == sf::Event::MouseButtonReleased) {
			dragging = false;

		}
		cout << "Dragging: " << dragging << " from " << ut::to_string(startDragPos) << endl;
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
		cout << ":";
		string cmd;
		getline(cin, cmd);
		istringstream iss(cmd);
		vector<string> tokens{ istream_iterator<string>{iss},
					  istream_iterator<string>{} };
		execute(tokens);
	}
}

void InputManager::handleKeyboardCommands(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
			sceneView->zoom(0.9);
			environment->renderRectPosition = sceneView->getCenter() - Vector2f(sceneView->getSize().x / 2, sceneView->getSize().y / 2);
			environment->renderRectSize = Vector2f(sceneView->getSize().x, sceneView->getSize().y);
			currentZoom = (sceneView->getSize().x / uiView->getSize().x + sceneView->getSize().y / uiView->getSize().y) / 2;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
			sceneView->zoom(1.1);
			environment->renderRectPosition = sceneView->getCenter() - Vector2f(sceneView->getSize().x / 2, sceneView->getSize().y / 2);
			environment->renderRectSize = Vector2f(sceneView->getSize().x, sceneView->getSize().y);
			currentZoom = (sceneView->getSize().x / uiView->getSize().x + sceneView->getSize().y / uiView->getSize().y) / 2;
		}
	}
}

void InputManager::execute(vector<string> tokens)
{
	if (tokens.size() == 0) return;
	string command = tokens[0];
	switch (resolveCommand(command)) {

	case set_behaviour:
		if (tokens.size() != 2)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		setBehaviour(tokens[1]); break;

	case set_collide:
		if (tokens.size() != 2)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		environment->entityCollision = tokens[1] == "true"; break;

	case show_zones:
		if (tokens.size() != 2)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		environment->showZones = tokens[1] == "true"; break;

	case show_ui:
		if (tokens.size() != 2)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		environment->showUI = tokens[1] == "true"; break;

	case show_lines:
		if (tokens.size() != 2)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		environment->showLines = tokens[1] == "true"; break;

	case set_gravity_center:
		if (tokens.size() != 3)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		setGravityCenter(tokens[1], tokens[2]); break;

	default:
		cout << "ERROR: Invalid Command" << endl;
	}
}
