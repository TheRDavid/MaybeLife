#include "Commander.h"

#include <iostream>
#include <memory>

#include "Environment.h"
#include "Grid.h"

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
	m_selectedZone = zone;
	m_environment->m_selectedZone = zone;
}

void Commander::selectZoneAt(sf::Vector2f position)
{
	setSelectedZone(m_environment->m_entityGrid->zoneAt(position));
}

void Commander::setSelectedEntity(std::shared_ptr<Entity> entity)
{
	m_selectedEntity = entity;
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
