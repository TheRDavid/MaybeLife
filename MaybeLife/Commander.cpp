#include "Commander.h"

#include <iostream>
#include <memory>

#include "Environment.h"
#include "Grid.h"
#include "Utilities.h"

float Commander::goodToBadRatio()
{
	return m_numGoodGuys / (m_numBadGuys + m_numGoodGuys);
}

void Commander::setEntityCollision(bool enabled)
{
	m_environment->m_entityCollision = enabled;
}

void Commander::setGUIVisible(bool visible)
{
	m_environment->m_showGUI = visible;
}

void Commander::setZonesVisible(bool visible)
{
	m_environment->m_showZones = visible;
}

void Commander::setSelectedZone(Zone * zone)
{
	m_environment->m_selectedZone = zone;
}

std::shared_ptr<Entity> Commander::entityAt(sf::Vector2f position)
{
	Zone* containerZone = m_environment->m_entityGrid->zoneAt(position);
	std::shared_ptr<Entity> closest = nullptr;
	float closestDistance = UT_MOUSE_SELECTION_NEGLIGIBLE_SPATIAL_DIFFERENCE;
	if (containerZone != nullptr)
	{
		for (auto entity : containerZone->m_entities)
		{
			float dist = ut::euclideanDistance(position, entity->m_position);
			if (dist < closestDistance)
			{
				closest = entity;
				closestDistance = dist;
			}
		}
	}
	return closest;
}

Zone* Commander::zoneAt(sf::Vector2f position)
{
	return m_environment->m_entityGrid->zoneAt(position);
}

void Commander::selectZoneAt(sf::Vector2f position)
{
	m_environment->m_selectedZone = zoneAt(position);
}

std::weak_ptr<Entity> Commander::getSelectedEntity()
{
	return m_environment->m_selectedEntity;
}

void Commander::setSelectedEntity(std::shared_ptr<Entity> entity)
{
	if (entity != nullptr)
	{
		m_environment->m_selectedEntity = entity;
	}
}

void Commander::deSelectEntity()
{
	m_environment->m_selectedEntity.reset();
}

void Commander::setSelectedZone_hover(Zone * zone)
{
	m_environment->m_hoveredZone = zone;
}

void Commander::selectZoneAt_hover(sf::Vector2f position)
{
	m_environment->m_hoveredEntity = entityAt(position);
}

void Commander::setSelectedEntity_hover(std::shared_ptr<Entity> entity)
{
	m_environment->m_hoveredEntity = entity;
}

void Commander::selectEntityAt_hover(sf::Vector2f position)
{
	m_environment->m_hoveredEntity = entityAt(position);
}

void Commander::selectEntityAt(sf::Vector2f position)
{
	auto newSelection = entityAt(position);
	if (newSelection != nullptr)
	{
		m_environment->m_selectedEntity = newSelection;
	}
}

void Commander::addEntity(std::shared_ptr<Entity> entity)
{
	entity->m_position = m_environment->legalizePosition(entity->m_position);
	Zone* zone = m_environment->m_entityGrid->zoneAt(entity->m_position);
	zone->addEntity(entity);
	//std::cout << "Adding entity" << std::endl << entity->to_string() << std::endl << "to zone" << std::endl << zone->to_string() << std::endl;
	m_environment->m_insertLock.lock();
	m_environment->m_entities->push_back(entity);
	m_environment->m_insertLock.unlock();
}

void Commander::deleteEntity(std::shared_ptr<Entity> entity)
{
	entity->m_enabled = false;
	entity->m_zone->removeEntity(entity);
	m_environment->m_insertLock.lock();
	m_environment->m_toRemove->push_back(entity);
	//m_environment->m_entities->erase(std::remove(m_environment->m_entities->begin(), m_environment->m_entities->end(), entity), m_environment->m_entities->end());
	m_environment->m_insertLock.unlock();
}

void Commander::playSimulation()
{
	m_environment->m_paused = false;
}
void Commander::pauseSimulation()
{
	m_environment->m_paused = true;
}
void Commander::stopSimulation()
{
	m_environment->m_initiateShutdown = true;
	m_environment->m_shutdownLock.lock();
	m_window->close();
}
void Commander::stepSimulation()
{
	m_environment->m_paused = false;
	m_environment->m_pauseAfterNextStep = true;
}
