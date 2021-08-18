#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Zone;
class Environment;
class Entity
{
public:

	enum Behaviour {
		SPREAD, RANDOM, GRAVITATE, SLEEP, FALL, GROUP
	};

	Environment* environment;
	Behaviour behaviour = RANDOM;
	Vector2f position, size;
	float majorSize;
	bool collide;

	int id = ID();
	Color color = Color::White;
	Zone* zone;
	Entity(Environment* environment, Behaviour behaviour, Vector2f position, Vector2f size, bool collide, Color color);
	Entity(Environment* environment, Behaviour behaviour, Vector2f position, Vector2f size, bool collide);
	Entity(Environment* environment, Behaviour behaviour, Vector2f position, Vector2f size);
	Entity(Environment* environment, Behaviour behaviour, Vector2f position);
	Entity(Environment* environment, Behaviour behaviour);
	Entity(Environment* environment);
	void update();
	void updateCollision();
	void actFall();
	void actGravitate();
	void actGroup();
	void actRandom();
	void actSpread();
	bool legalPosition_strict(Entity* entity, Vector2f position, Zone* zone);
	bool colliding(Entity* entity, Vector2f position, Zone* zone);
	bool colliding(Entity* entity, Zone* zone);
	std::string to_string();
	std::string to_bounds_string();
private:

	static int ID()
	{
		int ID = 0;
		return ID++;
	}
};

