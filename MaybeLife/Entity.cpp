#include "Entity.h"

#include <iostream>

#include "Zone.h"
#include "Utilities.h"
#include "Environment.h"
static unsigned long long int nextId = 0;

Entity::Entity(Environment* environment, sf::Vector2f position, sf::Vector2f size, bool collide, sf::Color color)
{
	this->m_environment = environment;
	this->m_position = position;
	this->m_size = size;
	this->m_color = color;
	this->m_collide = collide;

}
Entity::Entity(Environment* environment, sf::Vector2f position, sf::Vector2f size, bool collide) : Entity(environment, position, size, collide, sf::Color::White)
{
}
Entity::Entity(Environment* environment, sf::Vector2f position, sf::Vector2f size) : Entity(environment, position, size, true)
{
}
Entity::Entity(Environment* environment, sf::Vector2f position) : Entity(environment, position, sf::Vector2f(1, 1))
{
}
Entity::Entity(Environment* environment) : Entity(environment, sf::Vector2f(0, 0))
{
}
void Entity::update()
{
	if (m_environment->m_entityCollision && m_collide) {
		updateCollision();
	}
}
void Entity::updateCollision()
{
	if (colliding(shared_from_this(), m_position, m_zone)) {
		bool foundDodge = false;
		sf::Vector2f dodgePosition;

#pragma warning( suppress : 4244 ) // loss of precision (int to float) is wanted, as we're creating a random index
		int startDir = ut::randomNumber(0, 8), dirCount = 0;
		//std::cout << m_id << " collides, startDir = " << startDir << std::endl;
		while (dirCount++ < 8) {
			if (++startDir == 8) {
				startDir = 0;
			}
			dodgePosition = sf::Vector2f(m_environment->gridDirections[startDir].x * m_size.x, m_environment->gridDirections[startDir].y * m_size.y) + m_position;
			//std::cout << "Try dodgin to with " << startDir << " -> " << ut::to_string(environment->gridDirections[startDir]) << " to " << ut::to_string(dodgePosition) << std::endl;
			if (legalPosition_strict(shared_from_this(), dodgePosition, m_zone)) {
				foundDodge = true;
				break;
			}
		}
		if (foundDodge) {
			m_position = dodgePosition;
		}
	}
}
std::string Entity::to_bounds_string()
{
	return "@" + ut::to_string(m_position) + " sized " + ut::to_string(m_size);
}

sf::Vector2f Entity::bounceFromEdgeIfNecessary(sf::Vector2f dir)
{
	sf::Vector2f newDirection = dir;
	float minDist = m_size.x + m_size.y;

	if (m_position.x < minDist)
	{
		newDirection.x = 1;
	}
	else if (m_environment->m_size.x - m_position.x < minDist)
	{
		newDirection.x = -1;
	}

	if (m_position.y < minDist)
	{
		newDirection.y = 1;
	}
	else if (m_environment->m_size.y - m_position.y < minDist)
	{
		newDirection.y = -1;
	}

	return newDirection;
}

std::string Entity::to_string()
{
	return "Entity \"" + m_name+"\" (" + std::to_string(m_id) + ") " + to_bounds_string();
}
bool operator<(const Entity& l, const Entity& r)
{
	return l.m_id < r.m_id; // keep the same order
}
bool operator>(const Entity& l, const Entity& r)
{
	return l.m_id > r.m_id; // keep the same order
}

bool Entity::colliding(std::shared_ptr<Entity> entity, Zone * zone)
{
	return colliding(entity, entity->m_position, zone);
}


bool Entity::colliding(std::shared_ptr<Entity> entity, sf::Vector2f pos, Zone * zone)
{
	for (Zone* z : zone->m_neighbours) {
		for (std::shared_ptr<Entity> otherEntity : z->m_entities)
		{
			if (entity->m_id != otherEntity->m_id && otherEntity->m_collide
				&& abs(pos.x - otherEntity->m_position.x) < (entity->m_size.x + otherEntity->m_size.x)
				&& abs(pos.y - otherEntity->m_position.y) < (entity->m_size.y + otherEntity->m_size.y)
				)
			{
				//color = Color::Red;
				//std::cout << to_bounds_string() << " is stuck with " << otherEntity->to_bounds_string() << std::endl;
				return true;
			}
		}
	}
	//color = Color::White;
	return false;
}


bool Entity::legalPosition_strict(std::shared_ptr<Entity> entity, sf::Vector2f checkPosition, Zone* uZone)
{
	if (m_environment->m_entityCollision && m_collide && colliding(entity, checkPosition, uZone)) {
		return false;
	}
	return 0 < checkPosition.x && checkPosition.x < m_environment->m_size.x && 0 < checkPosition.y && checkPosition.y < m_environment->m_size.y;
}
