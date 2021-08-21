#pragma once
#include "Entity.h"

#include <SFML/Graphics.hpp>
class Person :
	public Entity
{
public:
	int m_health, m_speed;
	bool m_good;

	Person(Environment* environment, sf::Vector2f position, int health, int speed, bool good);

	void move(sf::Vector2f dir);
};

