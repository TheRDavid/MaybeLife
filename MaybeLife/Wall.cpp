#include "Wall.h"

Wall::Wall(Environment * environment, sf::Vector2f position)
	: Entity(environment, position, sf::Vector2f(4, 4))
{
	m_color = sf::Color(88, 255, 88, 120);
}
