#include "Commander.h"

#include <iostream>

#include "Environment.h"
#include "Grid.h"

Commander::Commander(Environment * environment, sf::RenderWindow * window, sf::View* sceneView, sf::View* uiView)
{
	this->m_environment = environment;
	this->m_window = window;
	this->m_sceneView = sceneView;
	this->m_guiView = uiView;
}

void Commander::setEntityCollision(bool enabled)
{
	m_environment->m_entityCollision = enabled;
}

void Commander::setUIVisible(bool visible)
{
	m_environment->m_showUI = visible;
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

void Commander::setSelectedEntity(Entity * entity)
{
	m_selectedEntity = entity;
}

void Commander::addEntity(sf::Vector2f position)
{
	Entity* entity = new Entity(m_environment, position, sf::Vector2f(4,4));
	Zone* zone = m_environment->m_entityGrid->zoneAt(position);
	zone->addEntity(entity);
	std::cout << "Adding entity" << std::endl << entity->to_string() << std::endl << "to zone" << std::endl << zone->to_string() << std::endl;
	m_environment->m_insertLock.lock();
	m_environment->m_entities->push_back(entity);
	m_environment->m_insertLock.unlock();
}
