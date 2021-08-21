#include "Environment.h"

#include <time.h>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <thread>
#include <iostream>

#include "AppConfig.h"
#include "Utilities.h"
#include "Commander.h"
#include "Grid.h"

Environment::Environment(sf::RenderWindow * renderWindow, sf::Vector2i size, int _numZones, int threads, sf::View* sceneView)
{
	this->m_sceneView = sceneView;
	this->m_size = size;
	m_numThreads = threads;
	steps = 0;
	int zoneCapacity = 2 * m_entities->size() / _numZones;

	m_entityGrid = new Grid(_numZones, zoneCapacity, m_size);

	m_showZones = m_entityGrid->m_numZones <= 1000;
	m_window = renderWindow;

	m_zoneLines = new sf::VertexArray(sf::Lines, (m_entityGrid->m_cols + m_entityGrid->m_rows) * 2);
}

void Environment::start(std::vector<Entity*>* entities) {
	this->m_entities = entities;


	for (Entity* entity : (*m_entities)) {
		entity->m_environment = this;
		Zone* zone = m_entityGrid->zoneAt(entity->m_position);
		zone->addEntityImmediatly(entity);
	}

	for (int i = 0; i < m_numThreads; i++) {
		int firstZone = i == 0 ? 0 : i * m_entityGrid->m_numZones / m_numThreads;
		int lastZone = i == 0 ? m_entityGrid->m_numZones / m_numThreads : (i + 1)*m_entityGrid->m_numZones / m_numThreads;
		std::cout << "New Thread: " << i << " zones " << firstZone << " to " << lastZone << std::endl;
		m_zoneProcessingRanges.push_back(sf::Vector2i(firstZone, lastZone));
	}
	new std::thread(&Environment::updateEntities, this);
}

bool Environment::legalPosition(sf::Vector2f position)
{
	return 0 <= position.x && position.x <= m_size.x && 0 <= position.y && position.y <= m_size.y;
}

void Environment::adjustRenderingRect()
{
	m_renderRectPosition = m_sceneView->getCenter() - sf::Vector2f(m_sceneView->getSize().x / 2, m_sceneView->getSize().y / 2);
	m_renderRectSize = sf::Vector2f(m_sceneView->getSize().x, m_sceneView->getSize().y);
}

void Environment::updateZoneRange(int firstZone, int lastZone)
{
	for (int i = firstZone; i < lastZone; i++)
	{
		Zone* uZone = m_entityGrid->m_zones[i];
		uZone->update();
		for (Entity* entity : uZone->m_entities)
		{
			entity->update();
		}
	}
}

void Environment::updateEntities()
{
	while (true) {
		int rangeIndex = 0;
		for (auto range : m_zoneProcessingRanges)
		{
			switch (rangeIndex++)
			{
			case 0: zoneProcessor_0 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 1: zoneProcessor_1 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 2: zoneProcessor_2 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 3: zoneProcessor_3 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 4: zoneProcessor_4 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 5: zoneProcessor_5 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 6: zoneProcessor_6 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 7: zoneProcessor_7 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 8: zoneProcessor_8 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 9: zoneProcessor_9 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 10: zoneProcessor_10 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 11: zoneProcessor_11 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 12: zoneProcessor_12 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 13: zoneProcessor_13 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 14: zoneProcessor_14 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 15: zoneProcessor_15 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			}
		}
		for (int i = 0; i < rangeIndex; i++)
		{
			switch (i)
			{
			case 0: zoneProcessor_0.wait();
				break;
			case 1: zoneProcessor_1.wait();
				break;
			case 2: zoneProcessor_2.wait();
				break;
			case 3: zoneProcessor_3.wait();
				break;
			case 4: zoneProcessor_4.wait();
				break;
			case 5: zoneProcessor_5.wait();
				break;
			case 6: zoneProcessor_6.wait();
				break;
			case 7: zoneProcessor_7.wait();
				break;
			case 8: zoneProcessor_8.wait();
				break;
			case 9: zoneProcessor_9.wait();
				break;
			case 10: zoneProcessor_10.wait();
				break;
			case 11: zoneProcessor_11.wait();
				break;
			case 12: zoneProcessor_12.wait();
				break;
			case 13: zoneProcessor_13.wait();
				break;
			case 14: zoneProcessor_14.wait();
				break;
			case 15: zoneProcessor_15.wait();
				break;
			}
		}
		steps++;
	}
}

void Environment::draw()
{
	m_window->setView(*m_sceneView);
	if (m_rects == NULL) {
		m_rects = new sf::VertexArray(sf::Quads, 4 * m_entities->size());
	}

	if (m_showZones)
		drawZones();

	m_rects->clear();
	int lineIndex = 0;
	int idx = 0;
	for (Entity* entity : *m_entities)
	{
		if (inRenderRect(entity)) {
			sf::Vector2f ePos = entity->m_position;
			sf::Vector2f eSize = entity->m_size;
			sf::Color col = entity->color;
			m_rects->append(sf::Vector2f(ePos.x - eSize.x, ePos.y - eSize.y));
			(*m_rects)[idx++].color = col;
			m_rects->append(sf::Vector2f(ePos.x + eSize.x, ePos.y - eSize.y));
			(*m_rects)[idx++].color = col;
			m_rects->append(sf::Vector2f(ePos.x + eSize.x, ePos.y + eSize.y));
			(*m_rects)[idx++].color = col;
			m_rects->append(sf::Vector2f(ePos.x - eSize.x, ePos.y + eSize.y));
			(*m_rects)[idx++].color = col;
		}
	}

	m_window->draw(*m_rects);
}

std::string Environment::stepsToString()
{
	std::string str = "Steps: ";
	for (int i = 0; i < std::max(1, m_numThreads); i++) {
		str += " " + std::to_string(steps);
	}
	return str;
}

void Environment::drawZones()
{

	if (m_selectedZone != nullptr) {
		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(m_selectedZone->xEnd - m_selectedZone->xStart, m_selectedZone->yEnd - m_selectedZone->yStart));
		rectangle.setOutlineColor(sf::Color::Red);
		rectangle.setOutlineThickness(1);
		rectangle.setFillColor(sf::Color(70, 70, 190, 100));
		rectangle.setPosition(m_selectedZone->xStart, m_selectedZone->yStart);
		m_window->draw(rectangle);
		for (Zone* neighbour : m_selectedZone->m_neighbours) {
			rectangle.setSize(sf::Vector2f(neighbour->xEnd - neighbour->xStart, neighbour->yEnd - neighbour->yStart));
			rectangle.setOutlineColor(sf::Color::Blue);
			rectangle.setOutlineThickness(1);
			rectangle.setFillColor(sf::Color(70, 70, 190, 50));
			rectangle.setPosition(neighbour->xStart, neighbour->yStart);
			m_window->draw(rectangle);
		}
	}

	int ldx = 0;
	sf::Color lC = sf::Color(150, 150, 150, 255);
	for (int i = 0; i < m_entityGrid->m_rows; i++) {
		(*m_zoneLines)[ldx].position = sf::Vector2f(0, i * m_entityGrid->m_zoneHeight);
		(*m_zoneLines)[ldx++].color = lC;
		(*m_zoneLines)[ldx].position = sf::Vector2f(m_size.x, i * m_entityGrid->m_zoneHeight);
		(*m_zoneLines)[ldx++].color = lC;
	}
	for (int i = 0; i < m_entityGrid->m_cols; i++) {
		(*m_zoneLines)[ldx].position = sf::Vector2f(i * m_entityGrid->m_zoneWidth, 0);
		(*m_zoneLines)[ldx++].color = lC;
		(*m_zoneLines)[ldx].position = sf::Vector2f(i * m_entityGrid->m_zoneWidth, m_size.y);
		(*m_zoneLines)[ldx++].color = lC;
	}
	m_window->draw(*m_zoneLines);
}

bool Environment::inRenderRect(Entity * entity)
{
	//std::cout << entity->to_bounds_string() + " in " << ut::to_string(m_renderRectPosition) << " x " << ut::to_string(m_renderRectSize) << std::endl;
	return
		// x overlapping?
		(entity->m_position.x + entity->m_size.x > m_renderRectPosition.x
			&& entity->m_position.x - entity->m_size.x < m_renderRectPosition.x + m_renderRectSize.x)
		&&
		// y overlapping?
		(entity->m_position.y + entity->m_size.y > m_renderRectPosition.y
			&& entity->m_position.y - entity->m_size.y < m_renderRectPosition.y + m_renderRectSize.y);
}

bool Environment::inRenderRect(Zone * zone)
{
	return
		// x overlapping?
		(zone->xEnd > m_renderRectPosition.x && zone->xStart < m_renderRectPosition.x + m_renderRectSize.x)
		&&
		// y overlapping?
		(zone->yEnd > m_renderRectPosition.y && zone->yStart < m_renderRectPosition.y + m_renderRectSize.y);
}


