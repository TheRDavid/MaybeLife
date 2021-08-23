#include "Fighter.h"


Fighter::Fighter(Environment* environment, sf::Vector2f position, sf::Vector2f size, float speed, bool good, float attack, float viewDistance, std::shared_ptr<Base> base)
	: Person(environment, position, size, speed, good, viewDistance, 0.1f, base), m_attack(attack)
{
	m_shape = TRIANGLE;
}

void Fighter::attack(Person * person)
{
	person->m_health -= m_attack;
}
