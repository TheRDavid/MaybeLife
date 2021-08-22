#include "Fighter.h"


Fighter::Fighter(Environment* environment, sf::Vector2f position, sf::Vector2f size, float health, float speed, bool good, float attack)
	: Person(environment, position, size, health, speed, good), m_attack(attack)
{
}

void Fighter::attack(Person * person)
{
	person->m_health -= m_attack;
}
