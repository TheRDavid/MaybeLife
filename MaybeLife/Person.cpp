#include "Person.h"

Person::Person(Environment* environment, sf::Vector2f position, sf::Vector2f size, float health, float speed, bool good)
	: Entity(environment, position, size), m_health(health), m_speed(speed), m_good(good)
{
}

void Person::move(sf::Vector2f dir)
{
	m_position += sf::Vector2f(m_speed * dir.x, m_speed * dir.y);
}
