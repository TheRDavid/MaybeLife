#include "Person.h"

Person::Person(Environment* environment, sf::Vector2f position, int health, int speed, bool good) : Entity(environment)
{
	this->m_position = position;
}

void Person::move(sf::Vector2f dir)
{
	m_position += sf::Vector2f(m_speed * dir.x, m_speed * dir.y);
}
