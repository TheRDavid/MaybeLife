#pragma once
#include "Entity.h"

#include <SFML/Graphics.hpp>
class Person :
	public Entity
{
public:
	float m_health, m_speed;
	bool m_good;

	Person(Environment* environment, sf::Vector2f position, sf::Vector2f size, float health, float speed, bool good);

	void move(sf::Vector2f dir);
};

