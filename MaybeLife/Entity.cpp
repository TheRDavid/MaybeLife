#include "Entity.h"
#include "Zone.h"
#include "Utilities.h"
#include "Environment.h"
static unsigned long long int nextId = 0;

Entity::Entity(Environment* environment, Behaviour behaviour, Vector2f position, Vector2f size, bool collide, Color color)
{
	this->environment = environment;
	this->position = position;
	this->size = size;
	this->color = color;
	this->collide = collide;
	this->behaviour = behaviour;
	majorSize = max(size.x, size.y);

}
Entity::Entity(Environment* environment, Behaviour behaviour, Vector2f position, Vector2f size, bool collide) : Entity(environment, behaviour, position, size, collide, Color::White)
{
}
Entity::Entity(Environment* environment, Behaviour behaviour, Vector2f position, Vector2f size) : Entity(environment, behaviour, position, size, true)
{
}
Entity::Entity(Environment* environment, Behaviour behaviour, Vector2f position) : Entity(environment, behaviour, Vector2f(0, 0), Vector2f(1, 1))
{
}
Entity::Entity(Environment* environment, Behaviour behaviour) : Entity(environment, behaviour, Vector2f(0,0))
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
		Vector2f dodgePosition;
		int startDir = ut::randomNumber(0,8), dirCount = 0;
		//cout << id << " collides, startDir = " << startDir << endl;
		while (dirCount++ < 8) {
			if (++startDir == 8) {
				startDir = 0;
			}
			dodgePosition = Vector2f(environment->gridDirections[startDir].x * size.x, environment->gridDirections[startDir].y * size.y) + position;
			//cout << "Try dodgin to with " << startDir << " -> " << ut::to_string(environment->gridDirections[startDir]) << " to " << ut::to_string(dodgePosition) << endl;
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
	Vector2f dir = Vector2f(0, size.y);
	Vector2f ePos = position + dir;

	if (legalPosition_strict(this, ePos, zone)) {
		position = ePos;
	}
	else {
		int randDir = rand() == 1 ? 1 : -1;
		dir = Vector2f(randDir * majorSize * 2, size.y);
		ePos = position + dir;

		if (legalPosition_strict(this, ePos, zone)) {
			position = ePos;
		}
		else {
			dir = Vector2f(-randDir * majorSize * 2, size.y);
			ePos = position + dir;

			if (legalPosition_strict(this, ePos, zone)) {
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
	if (legalPosition_strict(this, newPos, zone)) {
		position = newPos;
	}
}
void Entity::actRandom()
{
	Vector2f dir = Vector2f(ut::randomNumber(-1, 1), ut::randomNumber(-1, 1));
	Vector2f ePos = position + dir;

	if (legalPosition_strict(this, ePos, zone)) {
		position = ePos;
	}
}
void Entity::actSpread()
{
	int lineIndex = 0;
	float xDir = 0, yDir = 0;
	Vector2f ePos = position;
	Vector2f newPos = ePos;
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
	newPos = ePos + Vector2f(xDir, yDir);
	if (legalPosition_strict(this, newPos, zone)) {
		position = newPos;
	}
	else {
		newPos = ePos + Vector2f(0, yDir);
		if (legalPosition_strict(this, newPos, zone)) {
			position = newPos;
		}
	}
}
void Entity::actGroup()
{
	int lineIndex = 0;
	float xDir = 0, yDir = 0;
	Vector2f ePos = position;
	Vector2f newPos = ePos;
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
	newPos = ePos + Vector2f(xDir, yDir);
	if (legalPosition_strict(this, newPos, zone)) {
		position = newPos;
	}
	else {
		newPos = ePos + Vector2f(0, yDir);
		if (legalPosition_strict(this, newPos, zone)) {
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

bool Entity::colliding(Entity* entity, Zone * zone)
{
	return colliding(entity, entity->position, zone);
}


bool Entity::colliding(Entity* entity, Vector2f pos, Zone * zone)
{
	for (Zone* z : zone->neighbours) {
		for (Entity* otherEntity : z->entities)
		{
			if (entity->id != otherEntity->id
				&& fabs(pos.x - otherEntity->position.x) < (entity->size.x/2 + otherEntity->size.x / 2)
				&& fabs(pos.y - otherEntity->position.y) < (entity->size.y / 2 + otherEntity->size.y / 2)
				)
			{
				//color = Color::Red;
				//cout << to_bounds_string() << " is stuck with " << otherEntity->to_bounds_string() << endl;
				return true;
			}
		}
	}
	//color = Color::White;
	return false;
}


bool Entity::legalPosition_strict(Entity* entity, Vector2f checkPosition, Zone* uZone)
{
	if (environment->entityCollision && collide && colliding(entity, checkPosition, uZone)) {
		return false;
	}
	return 0 < checkPosition.x && checkPosition.x < environment->size.x && 0 < checkPosition.y && checkPosition.y < environment->size.y;
}
