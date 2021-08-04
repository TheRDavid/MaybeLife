#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Zone;
class Entity
{
public:
	Vector2f position, size;
	float majorSize;

	unsigned long long int id;
	Color color = Color::White;
	Zone* zone;

	Entity(unsigned long long int id, Vector2f position, Vector2f size, Color color);
	Entity(unsigned long long int id, Vector2f position, Vector2f size);
	Entity(unsigned long long int id, Vector2f position);
	std::string to_string();
	std::string to_bounds_string();
};

