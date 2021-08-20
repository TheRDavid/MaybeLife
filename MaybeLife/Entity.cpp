#include "Entity.h"

#include <iostream>

#include "Zone.h"
#include "Utilities.h"
#include "Environment.h"
static unsigned long long int nextId = 0;

Entity::Entity(Environment* environment, Behaviour behaviour, sf::Vector2f position, sf::Vector2f size, bool collide, sf::Color color)
{
	this->environment = environment;
	this->position = position;
	this->size = size;
	this->color = color;
	this->collide = collide;
	this->behaviour = behaviour;
	majorSize = std::max(size.x, size.y);

}
Entity::Entity(Environment* environment, Behaviour behaviour, sf::Vector2f position, sf::Vector2f size, bool collide) : Entity(environment, behaviour, position, size, collide, sf::Color::White)
{
}
Entity::Entity(Environment* environment, Behaviour behaviour, sf::Vector2f position, sf::Vector2f size) : Entity(environment, behaviour, position, size, true)
{
}
Entity::Entity(Environment* environment, Behaviour behaviour, sf::Vector2f position) : Entity(environment, behaviour, position, sf::Vector2f(1, 1))
{
}
Entity::Entity(Environment* environment, Behaviour behaviour) : Entity(environment, behaviour, sf::Vector2f(0, 0))
{
}
Entity::Entity(Environment* environment) : Entity(environment, RANDOM)
{
}
void Entity::update()
{
	switch (behaviour) {
	case Behaviour::FALL:
		actFall();
		break;
	case Behaviour::GRAVITATE:
		actGravitate();
		break;
	case Behaviour::GROUP:
		actGroup();
		break;
	case Behaviour::RANDOM:
		actRandom();
		break;
	case Behaviour::SPREAD:
		actSpread();
		break;
	default:break;
	}
	if (environment->entityCollision && collide) {
		updateCollision();
	}
}
void Entity::updateCollision()
{
	if (colliding(this, position, zone)) {
		bool foundDodge = false;
		sf::Vector2f dodgePosition;
		int startDir = ut::randomNumber(0, 8), dirCount = 0;
		//std::cout << id << " collides, startDir = " << startDir << std::endl;
		while (dirCount++ < 8) {
			if (++startDir == 8) {
				startDir = 0;
			}
			dodgePosition = sf::Vector2f(environment->gridDirections[startDir].x * size.x, environment->gridDirections[startDir].y * size.y) + position;
			//std::cout << "Try dodgin to with " << startDir << " -> " << ut::to_string(environment->gridDirections[startDir]) << " to " << ut::to_string(dodgePosition) << std::endl;
			if (legalPosition_strict(this, dodgePosition, zone)) {
				foundDodge = true;
				break;
			}
		}
		if (foundDodge) {
			position = dodgePosition;
		}
	}
}
void Entity::actFall()
{
	sf::Vector2f dir = sf::Vector2f(0, size.y);
	sf::Vector2f ePos = position + dir;

	if (legalPosition_strict(this, ePos, zone)) {
		position = ePos;
	}
	else {
		int randDir = rand() == 1 ? 1 : -1;
		dir = sf::Vector2f(randDir * majorSize * 2, size.y);
		ePos = position + dir;

		if (legalPosition_strict(this, ePos, zone)) {
			position = ePos;
		}
		else {
			dir = sf::Vector2f(-randDir * majorSize * 2, size.y);
			ePos = position + dir;

			if (legalPosition_strict(this, ePos, zone)) {
				position = ePos;
			}
		}
	}
}
void Entity::actGravitate()
{
	sf::Vector2f dir = sf::Vector2f(environment->gravityCenter.x - position.x, environment->gravityCenter.y - position.y);
	if (dir.x != 0) dir.x = dir.x > 0 ? 1 : -1;
	if (dir.y != 0) dir.y = dir.y > 0 ? 1 : -1;
	sf::Vector2f newPos = position + dir;
	if (legalPosition_strict(this, newPos, zone)) {
		position = newPos;
	}
}
void Entity::actRandom()
{
	sf::Vector2f dir = sf::Vector2f(ut::randomNumber(-1, 1), ut::randomNumber(-1, 1));
	sf::Vector2f ePos = position + dir;

	if (legalPosition_strict(this, ePos, zone)) {
		position = ePos;
	}
}
void Entity::actSpread()
{
	int lineIndex = 0;
	float xDir = 0, yDir = 0;
	sf::Vector2f ePos = position;
	sf::Vector2f newPos = ePos;
	for (Zone* zonen : zone->neighbours) {
		for (Entity* neighbour : zonen->entities)
		{
			float xd = ePos.x - neighbour->position.x;
			float yd = ePos.y - neighbour->position.y;
			float dist = pow(xd, 2) + pow(yd, 2);
			if (id != neighbour->id) {
				if (xd != 0)
					xd = (xd > 0 ? 1000 : -1000);
				if (yd != 0)
					yd = (yd > 0 ? 1000 : -1000);
				xDir += xd / dist;
				yDir += yd / dist;
			}
		}
	}
	if (xDir != 0) xDir = xDir > 0 ? 1 : -1;
	if (yDir != 0) yDir = yDir > 0 ? 1 : -1;
	newPos = ePos + sf::Vector2f(xDir, yDir);
	if (legalPosition_strict(this, newPos, zone)) {
		position = newPos;
	}
	else {
		newPos = ePos + sf::Vector2f(0, yDir);
		if (legalPosition_strict(this, newPos, zone)) {
			position = newPos;
		}
	}
}
void Entity::actGroup()
{
	int lineIndex = 0;
	float xDir = 0, yDir = 0;
	sf::Vector2f ePos = position;
	sf::Vector2f newPos = ePos;
	for (Zone* zone : zone->neighbours) {
		for (Entity* neighbour : zone->entities)
		{
			float xd = -ePos.x + neighbour->position.x;
			float yd = -ePos.y + neighbour->position.y;
			float dist = pow(xd, 2) + pow(yd, 2);
			if (id != neighbour->id) {
				if (xd != 0)
					xd = (xd > 0 ? 1000 : -1000);
				if (yd != 0)
					yd = (yd > 0 ? 1000 : -1000);
				xDir += xd * dist;
				yDir += yd * dist;
			}
		}
	}
	if (xDir != 0) xDir = xDir > 0 ? 1 : -1;
	if (yDir != 0) yDir = yDir > 0 ? 1 : -1;
	newPos = ePos + sf::Vector2f(xDir, yDir);
	if (legalPosition_strict(this, newPos, zone)) {
		position = newPos;
	}
	else {
		newPos = ePos + sf::Vector2f(0, yDir);
		if (legalPosition_strict(this, newPos, zone)) {
			position = newPos;
		}
	}
}
std::string Entity::to_bounds_string()
{
	return "@" + ut::to_string(position) + " sized " + ut::to_string(size);
}
Entity::Behaviour Entity::to_behaviour(std::string behaviour)
{
	Behaviour newBehaviour;
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
		newBehaviour = Entity::Behaviour::SLEEP;
	}
	return newBehaviour;
}
std::string Entity::to_string()
{
	return "Entity " + std::to_string(id) + " " + to_bounds_string();
}
bool operator<(const Entity& l, const Entity& r)
{
	return l.id < r.id; // keep the same order
}
bool operator>(const Entity& l, const Entity& r)
{
	return l.id > r.id; // keep the same order
}

bool Entity::colliding(Entity* entity, Zone * zone)
{
	return colliding(entity, entity->position, zone);
}


bool Entity::colliding(Entity* entity, sf::Vector2f pos, Zone * zone)
{
	for (Zone* z : zone->neighbours) {
		for (Entity* otherE : z->entities)
		{
			Entity otherEntity = *otherE;
			if (entity->id != otherEntity.id
				&& pow(pos.x - otherEntity.position.x,2) < (entity->size.x / 2 + otherEntity.size.x / 2)
				&& pow(pos.y - otherEntity.position.y,2) < (entity->size.y / 2 + otherEntity.size.y / 2)
				)
			{
				//color = Color::Red;
				//std::cout << to_bounds_string() << " is stuck with " << otherEntity->to_bounds_string() << std::endl;
				return true;
			}
		}
	}
	//color = Color::White;
	return false;
}


bool Entity::legalPosition_strict(Entity* entity, sf::Vector2f checkPosition, Zone* uZone)
{
	if (environment->entityCollision && collide && colliding(entity, checkPosition, uZone)) {
		return false;
	}
	return 0 < checkPosition.x && checkPosition.x < environment->size.x && 0 < checkPosition.y && checkPosition.y < environment->size.y;
}
