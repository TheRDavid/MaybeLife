#include "Peasant.h"

Peasant::Peasant(Environment * environment, sf::Vector2f position)
	: Worker(environment, position, sf::Vector2f(2, 1), 30, 2, false, 7)
{
	m_color = sf::Color(0, 0, 255, 100);
}