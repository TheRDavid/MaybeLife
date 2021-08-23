#include "Slave.h"

Slave::Slave(Environment * environment, sf::Vector2f position, std::shared_ptr<Base> base)
	: Worker(environment, position, sf::Vector2f(6,6), 2, false, 10, base)
{
	m_color = sf::Color(255, 0, 0, 100);
}
