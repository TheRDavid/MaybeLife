#include "Peasant.h"

Peasant::Peasant(Environment * environment, sf::Vector2f position, std::shared_ptr<Base> base)
	: Worker(environment, position, sf::Vector2f(6,6), 2, true, 10, base)
{
	m_color = sf::Color(100, 100, 255, 100);
}