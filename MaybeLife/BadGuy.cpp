#include "BadGuy.h"

BadGuy::BadGuy(Environment * environment, sf::Vector2f position)
	: Fighter(environment, position, sf::Vector2f(1,4), 60, 3.5f, false, 4)
{
	m_color = sf::Color(255, 0, 0, 255);
}
