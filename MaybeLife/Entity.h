#pragma once
#include <SFML/Graphics.hpp>
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
	sf::Vector2f position, size;
	float majorSize;
	bool collide;

	unsigned long long int id = ID();
	sf::Color color = sf::Color::White;
	Zone* zone;
	Entity(Environment* environment, Behaviour behaviour, sf::Vector2f position, sf::Vector2f size, bool collide, sf::Color color);
	Entity(Environment* environment, Behaviour behaviour, sf::Vector2f position, sf::Vector2f size, bool collide);
	Entity(Environment* environment, Behaviour behaviour, sf::Vector2f position, sf::Vector2f size);
	Entity(Environment* environment, Behaviour behaviour, sf::Vector2f position);
	Entity(Environment* environment, Behaviour behaviour);
	Entity(Environment* environment);
	void update();
	bool legalPosition_strict(Entity* entity, sf::Vector2f position, Zone* zone);
	std::string to_string();
	std::string to_bounds_string();
private:

	void updateCollision();
	void actFall();
	void actGravitate();
	void actGroup();
	void actRandom();
	void actSpread();
	bool colliding(Entity* entity, sf::Vector2f position, Zone* zone);
	bool colliding(Entity* entity, Zone* zone);
	static unsigned long long int ID()
	{
		static unsigned long long int ID = 0;
		return ID++;
	}
};

