#include "Base.h"

#include "GoodGuy.h"
#include "BadGuy.h"
#include "Commander.h"
#include "Slave.h"
#include "Peasant.h"

Base::Base(Environment * environment, sf::Vector2f position, bool good, float startNutrition)
	: Entity(environment, position, sf::Vector2f(12, 12)), m_nutrition(startNutrition)
{
	m_name = "Base";
	m_workerSpawnRate = 0.2f / 60;
	m_fighterSpawnRate = m_workerSpawnRate / 10;
	m_shape = CIRCLE;
	m_collide = false;
	m_good = good;
	if (good)
	{
		m_color = sf::Color(150, 150, 255, 255);
	}
	else {
		m_color = sf::Color(255, 150, 150, 255);
	}
}

void Base::update()
{
	if (m_nutrition > 100)
	{
		if (rand() / double(RAND_MAX) < m_workerSpawnRate)
		{
			std::cout << "Adding worker!" << std::endl;
			std::shared_ptr<Worker> worker;

			if (m_good)
			{
				worker = std::make_shared<Peasant>(m_environment, m_position, std::dynamic_pointer_cast<Base>(shared_from_this()));
			}
			else {
				worker = std::make_shared<Slave>(m_environment, m_position, std::dynamic_pointer_cast<Base>(shared_from_this()));
			}
			worker->m_base = std::dynamic_pointer_cast<Base>(shared_from_this());
			Commander::getInstance().addEntity(worker);
		}
		else if (rand() / double(RAND_MAX) < m_fighterSpawnRate) {
			//std::cout << "Adding fighter!" << std::endl;
			//std::shared_ptr<Fighter> fighter;

			if (m_good)
			{
			//	fighter = std::make_shared<GoodGuy>(m_environment, m_position, std::dynamic_pointer_cast<Base>(shared_from_this()));
			}
			else {
				//fighter = std::make_shared<BadGuy>(m_environment, m_position, std::dynamic_pointer_cast<Base>(shared_from_this()));
			}
			//fighter->m_base = std::dynamic_pointer_cast<Base>(shared_from_this());
			//Commander::getInstance().addEntity(fighter);
		}
	}
	Entity::update();
}
