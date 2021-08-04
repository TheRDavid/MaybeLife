#include "Entity.h"
#include "Zone.h"
#include "Utilities.h"
#include "Environment.h"
static unsigned long long int nextId = 0;

Entity::Entity(unsigned long long int id, Environment* environment, Behaviour behaviour, Vector2f position, Vector2f size, Color color)
{
	this->id = id;
	this->environment = environment;
	this->position = position;
	this->size = size;
	this->color = color;
	this->behaviour = behaviour;
	majorSize = max(size.x, size.y);

}
Entity::Entity(unsigned long long int id, Environment* environment, Behaviour behaviour, Vector2f position, Vector2f size)
{
}
Entity::Entity(unsigned long long int id, Environment* environment, Behaviour behaviour, Vector2f position) : Entity(id, environment, behaviour, Vector2f(0, 0), Vector2f(1,1))
{
}
Entity::Entity(unsigned long long int id, Environment* environment, Behaviour behaviour) : Entity(id, environment, behaviour, Vector2f(0,0))
{
}
Entity::Entity(unsigned long long int id, Environment* environment) : Entity(id, environment, RANDOM)
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
	case Behaviour::RANDOM:
		actRandom();
		break;
	case Behaviour::SPREAD:
		actSpread();
		break;
	default:break;
	}
}
void Entity::actFall()
{
	Vector2f dir = Vector2f(0, size.y);
	Vector2f ePos = position + dir;

	if (environment->legalPosition_strict(this, ePos, zone)) {
		position = ePos;
	}
	else {
		srand(time(NULL));
		int randDir = rand() == 1 ? 1 : -1;
		dir = Vector2f(randDir * majorSize * 2, size.y);
		ePos = position + dir;

		if (environment->legalPosition_strict(this, ePos, zone)) {
			position = ePos;
		}
		else {
			dir = Vector2f(-randDir * majorSize * 2, size.y);
			ePos = position + dir;

			if (environment->legalPosition_strict(this, ePos, zone)) {
				position = ePos;
			}
		}
	}
}
void Entity::actGravitate()
{
	Vector2f dir = Vector2f(environment->gravityCenter.x - position.x, environment->gravityCenter.y - position.y);
	if (dir.x != 0) dir.x = dir.x > 0 ? 1 : -1;
	if (dir.y != 0) dir.y = dir.y > 0 ? 1 : -1;
	Vector2f newPos = position + dir;
	if (environment->legalPosition_strict(this, newPos, zone)) {
		position = newPos;
	}
}
void Entity::actRandom()
{
	Vector2f dir = Vector2f(ut::randomNumber(-1, 1), ut::randomNumber(-1, 1));
	Vector2f ePos = position + dir;

	if (environment->legalPosition_strict(this, ePos, zone)) {
		position = ePos;
	}
}
void Entity::actSpread()
{
	int lineIndex = 0;
	float xDir = 0, yDir = 0;
	Vector2f ePos = position;
	Vector2f newPos = ePos;
	for (Zone* zone : zone->neighbours) {
		for (Entity* neighbour : zone->entities)
		{
			float xd = ePos.x - neighbour->position.x;
			float yd = ePos.y - neighbour->position.y;
			float dist = pow(xd, 2) + pow(yd, 2);
			if (id != neighbour->id) {
				if (xd != 0)
					xd = (xd > 0 ? 1000 : -1000) / dist;
				if (yd != 0)
					yd = (yd > 0 ? 1000 : -1000) / dist;
				xDir += xd / dist;
				yDir += yd / dist;
			}
		}
	}
	if (xDir != 0) xDir = xDir > 0 ? 1 : -1;
	if (yDir != 0) yDir = yDir > 0 ? 1 : -1;
	newPos = ePos + Vector2f(xDir, yDir);
	if (environment->legalPosition_strict(this, newPos, zone)) {
		position = newPos;
	}
	else {
		newPos = ePos + Vector2f(0, yDir);
		if (environment->legalPosition_strict(this, newPos, zone)) {
			position = newPos;
		}
	}
}
std::string Entity::to_bounds_string()
{
	return "@" + ut::to_string(position) + " sized " + ut::to_string(size);
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