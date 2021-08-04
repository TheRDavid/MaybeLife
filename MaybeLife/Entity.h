#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Zone;
class Environment;
class Entity
{
public:

	enum Behaviour {
		SPREAD, RANDOM, GRAVITATE, SLEEP, FALL
	};

	Environment* environment;
	Behaviour behaviour = RANDOM;
	Vector2f position, size;
	float majorSize;

	unsigned long long int id;
	Color color = Color::White;
	Zone* zone;

	Entity(unsigned long long int id, Behaviour behaviour, Vector2f position, Vector2f size, Color color);
	Entity(unsigned long long int id, Behaviour behaviour, Vector2f position, Vector2f size);
	Entity(unsigned long long int id, Behaviour behaviour, Vector2f position);
	Entity(unsigned long long int id, Behaviour behaviour);
	Entity(unsigned long long int id);
	void update();
	void actFall();
	void actGravitate();
	void actRandom();
	void actSpread();
	std::string to_string();
	std::string to_bounds_string();
};

