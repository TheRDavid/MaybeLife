#include "Person.h"

#include "Environment.h"
#include "Grid.h"
#include "Utilities.h"
#include "Commander.h"

Person::Person(Environment* environment, sf::Vector2f position, sf::Vector2f size, float speed, bool good, float viewDistance, float wanderStrength, std::shared_ptr<Base> base)
	: Entity(environment, position, size),
	m_health(100), m_speed(speed), m_good(good), m_viewDistance(viewDistance), m_wanderStrength(wanderStrength), m_base(base)
{
	m_gazeDirection = sf::Vector2f((float)(rand() / RAND_MAX * 360), (float)(rand() / RAND_MAX * 360));
	m_wanderDirection = sf::Vector2f(ut::randomNumber(-1, 1), ut::randomNumber(-1, 1));
}

void Person::move(sf::Vector2f dir)
{
	m_position += ut::normalize(dir) * m_speed;
}

void Person::wander()
{
	m_wanderDirection = bounceFromEdgeIfNecessary(m_wanderDirection);

	float newTurn = ut::randomNumber(-10, 10);
	float newTurnPart = m_wanderStrength * newTurn;
	float lastTurnPart = (1 - m_wanderStrength) * lastTurn;
	float turn = newTurnPart + lastTurnPart;
	m_wanderDirection = ut::rotate(m_wanderDirection, turn);

	lastTurn = turn;
	move(m_wanderDirection);
}

void Person::walkToBase()
{
	m_wanderDirection = m_base->m_position - m_position;
	move(m_wanderDirection);
}

void Person::update()
{
	m_health -= 0.05f;
	float healthNeeded = 100 - m_health;
	if (m_health <= 0)
	{
		m_health = 0;
		m_enabled = false;
		Commander::getInstance().deleteEntity(shared_from_this());
	}
	else if (healthNeeded > 80 && m_base->m_nutrition > 0) // run to base to feed
	{
		walkToBase();
		if (ut::manhattenDistance(m_position, m_base->m_position) < 4)
		{
			float healthProvided = std::min(healthNeeded, m_base->m_nutrition);
			m_health += healthProvided;
			m_base->m_nutrition -= healthProvided;
		}
	}

	viewLock.lock();
	//m_inViewDistance.clear();
	m_environment->m_entityGrid->closebyEntities(m_id, &m_inViewDistance, m_zone, m_position, m_viewDistance);

	toAddLock.lock();
	for (auto add : m_toAdd)
	{
		if (add.second != nullptr && add.second->m_enabled)
		{
			m_inViewDistance[add.first] = add.second;
		}
	}
	m_toAdd.clear();
	toAddLock.unlock();

	while (m_inViewDistance.size() > maxInMind)
	{
		auto iterator = m_inViewDistance.begin();
		std::advance(iterator, rand() % m_inViewDistance.size());
		m_inViewDistance.erase(iterator->first);
	}
	toRemoveLock.lock();
	m_toRemove.clear();
	for (auto e : m_inViewDistance)
	{
		if (e.second.expired())
		{
			m_toRemove.insert(e.first);
		}
		else {
			auto entity = e.second.lock();
			if (!entity->m_enabled)
			{
				std::cout << "Removing, as it's not enabled" << std::endl;
				m_toRemove.insert(e.first);
			}
			else if (auto person = std::dynamic_pointer_cast<Person>(entity))
			{
				if (ut::manhattenDistance(m_position, person->m_position) > m_viewDistance)
				{
					m_toRemove.insert(e.first);
				}
			}
		}
	}
	for (auto enitity_id : m_toRemove)
	{
		m_inViewDistance.erase(enitity_id);
	}
	toRemoveLock.unlock();
	viewLock.unlock();
	Entity::update();
}

void Person::jsonify(nlohmann::json * data)
{
	Entity::jsonify(data);
	(*data)["health"] = m_health;
	(*data)["base"] = m_base->m_id;
	(*data)["speed"] = m_speed;
	(*data)["viewDistance"] = m_viewDistance;
	(*data)["wanderStrength"] = m_wanderStrength;
	(*data)["good"] = m_good;
	(*data)["maxInMind"] = maxInMind;
	nlohmann::json entitiesInView;
	viewLock.lock();
	int idn = 0;
	for (auto inView : m_inViewDistance)
	{
		entitiesInView[std::to_string(idn++)] = inView.first;
	}
	viewLock.unlock();
	(*data)["entitiesInView"] = entitiesInView;
}
