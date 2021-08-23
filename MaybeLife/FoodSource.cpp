#include "FoodSource.h"

#include <iostream>
#include <math.h>

#include "Commander.h"
#include "Environment.h"
#include "FoodItem.h"
FoodSource::FoodSource(Environment * environment, sf::Vector2f position)
	: Entity(environment, position, sf::Vector2f(20, 20))
{
	m_shape = DIAMOND;
	m_color = sf::Color(255, 255, 50, 255);
	m_radius = rand() % 60 + 10;
	m_spawnRate = rand() / double(RAND_MAX) / 3;
	m_lifeSpan = rand() % 300 + 150;
}

void FoodSource::update()
{
	if (m_lifeSpan-- <= 0)
	{
		std::shared_ptr<FoodSource> newFoodSource = std::make_shared<FoodSource>(m_environment, sf::Vector2f(rand() % m_environment->m_size.x, rand() % m_environment->m_size.y));
		Commander::getInstance().addEntity(newFoodSource);
		Commander::getInstance().deleteEntity(shared_from_this());
	}
	else
	{
		if (rand() / double(RAND_MAX) < m_spawnRate)
		{
			float xOffset = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)) * 2 * m_radius - m_radius;
			float yOffset = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)) * 2 * m_radius - m_radius;
			sf::Vector2f foodPosition = sf::Vector2f(m_position.x + xOffset, m_position.y + yOffset);
			std::shared_ptr<FoodItem> food = std::make_shared<FoodItem>(m_environment, foodPosition);
			Commander::getInstance().addEntity(food);
		}
	}
}
