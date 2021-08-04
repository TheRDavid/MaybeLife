#include "Entity.h"
#include "Zone.h"
#include "Utilities.h"
static unsigned long long int nextId = 0;

Entity::Entity(unsigned long long int id, Vector2f position, Vector2f size) :Entity(id, position, Vector2f(1, 1), Color::White)
{
}
Entity::Entity(unsigned long long int id, Vector2f position, Vector2f size, Color color)
{
	this->id = id;
	this->position = position;
	this->size = size;
	this->color = color;
	majorSize = max(size.x, size.y);

}
Entity::Entity(unsigned long long int id, Vector2f position):Entity(id, position, Vector2f(1,1))
{
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