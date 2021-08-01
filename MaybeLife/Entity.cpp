#include "Entity.h"
#include "Zone.h"
static unsigned long long int nextId = 0;

Entity::Entity(unsigned long long int id, Vector2f position, Vector2f size)
{
	this->id = id;
	this->position = position;
	this->size = size;
}
Entity::Entity(unsigned long long int id, Vector2f position):Entity(id, position, Vector2f(1,1))
{
}
std::string Entity::toString()
{
	return "Entity " + std::to_string(id)+" [" + std::to_string(position.x) + ", " + std::to_string(position.y) + "] size: " + std::to_string(size.x) + "x" + std::to_string(size.y);
}
bool operator<(const Entity& l, const Entity& r)
{
	return l.id < r.id; // keep the same order
}
bool operator>(const Entity& l, const Entity& r)
{
	return l.id > r.id; // keep the same order
}