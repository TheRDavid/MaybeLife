#include "GoodGuy.h"

GoodGuy::GoodGuy(Environment * environment, sf::Vector2f position, std::shared_ptr<Base> base)
	: Fighter(environment, position, sf::Vector2f(11,11), 3, true, 3.5, 120, base)
{
	m_color = sf::Color(100, 100, 255, 255);
}
