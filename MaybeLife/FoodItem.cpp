#include "FoodItem.h"

FoodItem::FoodItem(Environment * environment, sf::Vector2f position)
	: Entity(environment, position)
{
	m_color = sf::Color(255, 255, 111, 255);
	m_nutrition = rand() % 3 + 1;
}
