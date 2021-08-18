#include "Zone.h"
#include "Environment.h"
#include "Entity.h"

Zone::Zone(unsigned long long int id, Environment* environment, float xStart, float xEnd, float yStart, float yEnd, int capacity)
{
	toRemove.reserve(25);
	this-> id = id;
	this->environment = environment;
	this->xStart = xStart;
	this->xEnd = xEnd;
	this->yStart = yStart;
	this->yEnd = yEnd;
	xMid = xStart + (xEnd - xStart) / 2;
	yMid = yStart + (yEnd - yStart) / 2;
}

void Zone::update()
{
	toRemove.clear();
	for (auto kvp : entities) {
		Entity* entity = kvp.second;
		if (!legalPosition(entity->position)) {
			toRemove.push_back(entity);
			Zone* newZone = environment->zoneAt(entity->position);
			if (newZone == nullptr) {
				cout << "Illegal Entity: " << entity->to_string() << endl;
			}
			else {
				newZone->entities.insert(std::pair<unsigned long long, Entity*>(entity->id,entity));
				entity->zone = newZone;
			}
		}
	}
	for (auto &entity : toRemove) {
		entities.erase(entity->id);
	}
}

bool Zone::legalPosition(Vector2f position)
{
	return xStart <= position.x && position.x <= xEnd && yStart <= position.y && position.y <= yEnd;
}

std::string Zone::toString()
{
	return "Zone [" + std::to_string(xStart) + ", " + std::to_string(yStart) + ", " + std::to_string(xEnd) + ", " + std::to_string(yEnd) + "] #" + std::to_string(entities.size());
}
