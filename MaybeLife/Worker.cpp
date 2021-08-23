#include "Worker.h"

#include <limits>

#include "Utilities.h"
#include "Commander.h"

Worker::Worker(Environment * environment, sf::Vector2f position, sf::Vector2f size, float speed, bool good, float carryStrength, std::shared_ptr<Base> base)
	: Person(environment, position, size, speed, good, 100, 0.05f, base), m_carryStrength(carryStrength), m_gatheredNutrition(0)
{
	lastTurn = ut::randomNumber(-10, 10);
}

void Worker::pickUp(std::shared_ptr<FoodItem> food)
{
	m_gatheredNutrition = std::min(m_carryStrength, m_gatheredNutrition + food->m_nutrition);
	Commander::getInstance().deleteEntity(food);
}

void Worker::update()
{
	Person::update();
	float healthNeeded = 100 - m_health;
	if (healthNeeded > 80 && m_base->m_nutrition > 0) // feed self
	{
		walkToBase();
		if (ut::manhattenDistance(m_position, m_base->m_position) < 4)
		{
			float provided = std::min(healthNeeded, m_base->m_nutrition);
			m_base->m_nutrition -= provided;
			m_health += provided;
		}
	}
	else
		if (healthNeeded > 60 && m_good) // feed self
		{
			float healthProvided = std::min(healthNeeded, m_gatheredNutrition);
			m_gatheredNutrition -= healthProvided;
			m_health += healthProvided;
		}

	{ // roam

		if (m_gatheredNutrition >= m_carryStrength) {
			walkToBase();
			if (ut::manhattenDistance(m_position, m_base->m_position) < 4)
			{
				m_base->m_nutrition += m_gatheredNutrition;
				m_gatheredNutrition = 0;
			}
		}
		else
		{
			std::shared_ptr<FoodItem> closestFood = nullptr;
			float closestDistance = std::numeric_limits<float>::max();
			for (auto kvp : m_inViewDistance)
			{
				auto entity = kvp.second;
				if (auto food = std::dynamic_pointer_cast<FoodItem>(entity.lock()))
				{
					// tell all your friends!
					for (auto kvp0 : m_inViewDistance)
					{
						auto entity = kvp0.second;
						if (auto person = std::dynamic_pointer_cast<Person>(entity.lock()))
						{
							if (person->m_good == m_good && person->m_id != m_id)
							{
								person->viewLock.lock();
								person->m_inViewDistance[food->m_id] = food;
								person->viewLock.unlock();
							}
						}
					}

					float distance = ut::manhattenDistance(m_position, food->m_position);
					// TODO: Remember distance when calculating in closebyEntities
					if (distance < closestDistance)
					{
						closestFood = food;
						closestDistance = distance;
					}
				}
			}


			if (closestFood != nullptr) {
				if (closestDistance < 4)
				{
					pickUp(closestFood);
				}
				else {
					m_wanderDirection = closestFood->m_position - m_position;
					move(m_wanderDirection);
				}
			}
			else {
				wander();
			}
		}
	}

}
