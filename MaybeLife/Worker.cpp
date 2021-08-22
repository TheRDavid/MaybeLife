#include "Worker.h"

Worker::Worker(Environment * environment, sf::Vector2f position, sf::Vector2f size, float health, float speed, bool good, float carryStrength)
	: Person(environment, position, size, health, speed, good), m_carryStrength(carryStrength)
{
}

void Worker::pickUp(FoodItem * food)
{
	m_gatheredNutrition = std::max(m_carryStrength, m_gatheredNutrition + food->m_nutrition);
	delete(food);
}
