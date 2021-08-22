#include "Slave.h"

Slave::Slave(Environment * environment, sf::Vector2f position)
	: Worker(environment, position, sf::Vector2f(1,2), 20, 1.5f, false, 5)
{
	m_color = sf::Color(255, 0, 0, 100);
}
