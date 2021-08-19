#include "Commander.h"

#include <iostream>

#include "Environment.h"
#include "Grid.h"

Commander::Commander(Environment * environment, sf::RenderWindow * window, sf::View* sceneView, sf::View* uiView)
{
	this->environment = environment;
	this->window = window;
	this->sceneView = sceneView;
	this->uiView = uiView;
}

void Commander::setEntityCollision(bool enabled)
{
	environment->entityCollision = enabled;
}

void Commander::setBehaviour(std::string behaviour)
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

void Commander::setUIVisible(bool visible)
{
	environment->showUI = visible;
}

void Commander::setZonesVisible(bool visible)
{
	environment->showZones = visible;
}

void Commander::setGravityCenter(std::string x, std::string y)
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

void Commander::setSelectedZone(Zone * zone)
{
	selectedZone = zone;
}

void Commander::selectZoneAt(sf::Vector2f position)
{
	setSelectedZone(environment->entityGrid->zoneAt(position));
}

void Commander::setSelectedEntity(Entity * entity)
{
	selectedEntity = entity;
}

void Commander::addEntity(sf::Vector2f position)
{
	Entity* entity = new Entity(environment, Entity::Behaviour::RANDOM, position);
	Zone* zone = environment->entityGrid->zoneAt(position);
	zone->addEntity(entity);
	std::cout << "Adding entity" << std::endl << entity->to_string() << std::endl << "to zone" << std::endl << zone->toString() << std::endl;
	environment->insertLock.lock();
	environment->entities->push_back(entity);
	environment->insertLock.unlock();
}
