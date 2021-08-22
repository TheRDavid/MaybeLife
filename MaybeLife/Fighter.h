#pragma once
#include "Person.h"
class Fighter :
	public Person
{
public:
	float m_attack;
	Fighter(Environment* environment, sf::Vector2f position, sf::Vector2f size, float health, float speed, bool good, float attack);

	void attack(Person* person);

};

