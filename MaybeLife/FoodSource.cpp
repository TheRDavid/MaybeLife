#include "FoodSource.h"

#include <iostream>

#include "Commander.h"
#include "Environment.h"
FoodSource::FoodSource(Environment * environment, sf::Vector2f position)
	: Entity(environment, position, sf::Vector2f(5,5))
{
	m_radius = rand() % 30 + 1;
	m_spawnRate = rand() % 1 / 10;
	m_lifeSpan = rand() % 60 + 30;
}

void FoodSource::update()
{
	std::cout << "Lifespan " << m_lifeSpan << std:: endl;
	if (m_lifeSpan-- <= 0)
	{
		//FoodSource* newFoodSource = new FoodSource(m_environment, sf::Vector2f(rand() % m_environment->m_size.x, rand() % m_environment->m_size.y));
		//Commander::getInstance().addEntity(newFoodSource);
		Commander::getInstance().deleteEntity(this);
	}
}
