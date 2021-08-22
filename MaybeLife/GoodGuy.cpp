#include "GoodGuy.h"

GoodGuy::GoodGuy(Environment * environment, sf::Vector2f position)
	: Fighter(environment, position, sf::Vector2f(4, 1), 70, 3, true, 3.5)
{
	m_color = sf::Color(50, 50, 255, 255);
}
