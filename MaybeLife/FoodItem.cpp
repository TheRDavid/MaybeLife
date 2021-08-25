#include "FoodItem.h"

FoodItem::FoodItem(Environment * environment, sf::Vector2f position)
	: Entity(environment, position)
{
	m_color = sf::Color(255, 255, 111, 255);
	m_nutrition = (float)(rand() % 3 + 1);
	m_name = "FoodItem (" + std::to_string(m_nutrition) + ")";
	m_size = sf::Vector2f(1, 1);
	m_collide = false;
}
