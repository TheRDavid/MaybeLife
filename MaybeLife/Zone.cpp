#include "Zone.h"

#include <algorithm>
#include <iostream>

#include "Grid.h"
#include "Entity.h"

Zone::Zone(Grid* grid, float xStart, float xEnd, float yStart, float yEnd, int capacity)
{
	toRemove.reserve(25);
	this->grid = grid;
	this->xStart = xStart;
	this->xEnd = xEnd;
	this->yStart = yStart;
	this->yEnd = yEnd;
	xMid = xStart + (xEnd - xStart) / 2;
	yMid = yStart + (yEnd - yStart) / 2;
}

void Zone::update()
{
	entityAccess.lock();
	for (auto entity : toAdd) {
		entities.push_back(entity);
	}
	toAdd.clear();
	entityAccess.unlock();

	toRemove.clear();
	for (Entity* entity : entities) {
		if (!legalPosition(entity->position)) {
			toRemove.push_back(entity);
			Zone* newZone = grid->zoneAt(entity->position);
			if (newZone == nullptr) {
				std::cout << "Illegal Entity: " << entity->to_string() << std::endl;
			}
			else {
				newZone->entities.push_back(entity);
				entity->zone = newZone;
			}
		}
	}
	for (auto entity : toRemove) {
		entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
	}
}

bool Zone::legalPosition(sf::Vector2f position)
{
	return xStart <= position.x && position.x <= xEnd && yStart <= position.y && position.y <= yEnd;
}

std::string Zone::to_string()
{
	return "Zone " + std::to_string(id) + " [" + std::to_string(xStart) + ", " + std::to_string(yStart) + ", " + std::to_string(xEnd) + ", " + std::to_string(yEnd) + "] #" + std::to_string(entities.size());
}

void Zone::addEntity(Entity * entity)
{
	entity->zone = this;
	entityAccess.lock();
	toAdd.push_back(entity);
	entityAccess.unlock();
}

void Zone::addEntityImmediatly(Entity * entity)
{
	entity->zone = this;
	entities.push_back(entity);
}
