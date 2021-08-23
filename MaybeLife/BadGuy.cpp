#include "BadGuy.h"

BadGuy::BadGuy(Environment * environment, sf::Vector2f position, std::shared_ptr<Base> base)
	: Fighter(environment, position, sf::Vector2f(11,11), 3.5f, false, 4, 80, base)
{
	m_color = sf::Color(255, 0, 0, 255);
}
