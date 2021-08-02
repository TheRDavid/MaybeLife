#include "Entities.h"

Entities::Entities(int capacity)
{
	positions->reserve(capacity);
	sizes->reserve(capacity);
	colors->reserve(capacity);
	zones->reserve(capacity);
}

void Entities::add(Vector2f position, Vector2f size, Color color)
{
	positions->push_back(position);
	sizes->push_back(size);
	colors->push_back(color);
}

std::string Entities::toString(int id)
{
	return "Entity " + std::to_string(id) + " [" + std::to_string(positions->at(id).x) + ", " + std::to_string(positions->at(id).y) + "] size: " + std::to_string(sizes->at(id).x) + "x" + std::to_string(sizes->at(id).y);
}
