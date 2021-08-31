#include "Grid.h"

#include <math.h>
#include <iostream>

#include "Zone.h"
#include "Entity.h"
#include "Utilities.h"

Grid::Grid(int _numZones, int zoneCapacity, sf::Vector2i size) {
	this->m_size = size;
	m_rows = std::max(1, (int)sqrt(_numZones / (m_size.x / m_size.y)));
	m_cols = _numZones / m_rows;
	m_numZones = m_cols * m_rows;
	m_zones.reserve(m_numZones);
	m_zoneWidth = m_size.x / (float)m_cols;
	m_zoneHeight = m_size.y / (float)m_rows;
	m_zoneDiagonal = sqrt(pow(m_zoneWidth, 2) + pow(m_zoneHeight, 2));

	for (int i = 0; i < m_numZones; i++) {
		int row = (int)(i / m_cols);
		int col = (int)(i - row * m_cols);
		float xPos = col * m_zoneWidth, yPos = row * m_zoneHeight;
		m_zones.push_back(new Zone(this, xPos, xPos + m_zoneWidth, yPos, yPos + m_zoneHeight, zoneCapacity, row, col));
	}
	for (int i = 0; i < m_numZones; i++) {
		Zone* zone = m_zones[i];
		zone->m_neighbours = neighbours(zone);
	}

	std::cout << "Created Grid: \n" << to_string() << std::endl;
}

std::vector<Zone*> Grid::neighbours(Zone* zone)
{
	std::vector<Zone*> m_neighbours;
	m_neighbours.push_back(zone);

	Zone* neighbour = zoneAt(sf::Vector2f(zone->xMid - m_zoneWidth, zone->yMid));
	if (neighbour != nullptr) {
		m_neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid, zone->yMid - m_zoneHeight));
	if (neighbour != nullptr) {
		m_neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid, zone->yMid + m_zoneHeight));
	if (neighbour != nullptr) {
		m_neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid + m_zoneWidth, zone->yMid));
	if (neighbour != nullptr) {
		m_neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid + m_zoneWidth, zone->yMid + m_zoneHeight));
	if (neighbour != nullptr) {
		m_neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid - m_zoneWidth, zone->yMid - m_zoneHeight));
	if (neighbour != nullptr) {
		m_neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid + m_zoneWidth, zone->yMid - m_zoneHeight));
	if (neighbour != nullptr) {
		m_neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid - m_zoneWidth, zone->yMid + m_zoneHeight));
	if (neighbour != nullptr) {
		m_neighbours.push_back(neighbour);
	}

	return m_neighbours;
}

Zone* Grid::zoneAt(sf::Vector2f position)
{
	if (!legalPosition(position)) {
		return nullptr;
	}
	int index = (int)(position.y / m_zoneHeight) * m_cols + (int)(position.x / m_zoneWidth);
	return m_zones[index];
}

bool Grid::legalPosition(sf::Vector2f position)
{
	return 0 <= position.x && position.x <= m_size.x && 0 <= position.y && position.y <= m_size.y;
}

std::string Grid::to_string()
{
	return "Grid (" + std::to_string(m_numZones) + "), size=" + ut::to_string(m_size) + ", cols="
		+ std::to_string(m_cols) + ", rows=" + std::to_string(m_rows) + ", zone width=" + std::to_string(m_zoneWidth)
		+ ", zone height=" + std::to_string(m_zoneHeight);
}

void Grid::closebyEntities(int id, std::map<int, std::weak_ptr<Entity>>* list, Zone * zone, sf::Vector2f position, float distance)
{
	for (auto entity : zone->m_entities)
	{
		if (entity->m_id != id && entity->m_enabled && ut::manhattenDistance(position, entity->m_position) < distance)
		{
			(*list)[entity->m_id] = entity;
		}
	}

	float x_remain = distance;
	float y_remain = distance;
	float d_remain = distance;
	// TODO: compress code, jesus christ

	// check vertical
	int nextUpperRow = zone->m_row;
	int nextLowerRow = zone->m_row;

	while (y_remain > 0)
	{
		y_remain -= m_zoneHeight;
		nextUpperRow--;
		nextLowerRow++;

		// TODO: optimize, if zone is very close, don't check entities for distances
		if (nextUpperRow >= 0)
		{
			for (auto entity : m_zones[nextUpperRow * m_cols + zone->m_col]->m_entities)
			{
				if (entity->m_id != id && entity->m_enabled && ut::manhattenDistance(position, entity->m_position) < distance)
				{
					(*list)[entity->m_id] = entity;
				}
			}
		}
		if (nextLowerRow < m_rows)
		{
			for (auto entity : m_zones[nextLowerRow * m_cols + zone->m_col]->m_entities)
			{
				if (entity->m_id != id && entity->m_enabled && ut::manhattenDistance(position, entity->m_position) < distance)
				{
					(*list)[entity->m_id] = entity;
				}
			}
		}
	}

	// check horizontal
	int nextLeftCol = zone->m_col;
	int nextRightCol = zone->m_col;

	while (x_remain > 0)
	{
		x_remain -= m_zoneWidth;
		nextLeftCol--;
		nextRightCol++;

		if (nextLeftCol >= 0)
		{
			for (auto entity : m_zones[zone->m_row * m_cols + nextLeftCol]->m_entities)
			{
				if (entity->m_id != id && entity->m_enabled && ut::manhattenDistance(position, entity->m_position) < distance)
				{
					(*list)[entity->m_id] = entity;
				}
			}
		}
		if (nextRightCol < m_cols)
		{
			for (auto entity : m_zones[zone->m_row * m_cols + nextRightCol]->m_entities)
			{
				if (entity->m_id != id && entity->m_enabled && ut::manhattenDistance(position, entity->m_position) < distance)
				{
					(*list)[entity->m_id] = entity;
				}
			}
		}
	}

	// check diagonal

	// TODO simplify, shouldn't need that many
	int nextUpperLeftCol = zone->m_col;
	int nextUpperRightCol = zone->m_col;
	int nextUpperLeftRow = zone->m_row;
	int nextUpperRightRow = zone->m_row;

	int nextLowerLeftCol = zone->m_col;
	int nextLowerRightCol = zone->m_col;
	int nextLowerLeftRow = zone->m_row;
	int nextLowerRightRow = zone->m_row;

	while (d_remain > 0)
	{
		d_remain -= m_zoneDiagonal;

		nextUpperLeftCol--;
		nextUpperRightCol++;
		nextUpperLeftRow--;
		nextUpperRightRow--;

		nextLowerLeftCol--;
		nextLowerRightCol++;
		nextLowerLeftRow++;
		nextLowerRightRow++;

		if (nextUpperLeftCol >= 0 && nextUpperLeftRow >= 0)
		{
			for (auto entity : m_zones[nextUpperLeftRow * m_cols + nextUpperLeftCol]->m_entities)
			{
				if (entity->m_id != id && entity->m_enabled && ut::manhattenDistance(position, entity->m_position) < distance)
				{
					(*list)[entity->m_id] = entity;
				}
			}
		}

		if (nextUpperRightCol < m_cols && nextUpperRightRow >= 0)
		{
			for (auto entity : m_zones[nextUpperRightRow * m_cols + nextUpperRightCol]->m_entities)
			{
				if (entity->m_id != id && entity->m_enabled && ut::manhattenDistance(position, entity->m_position) < distance)
				{
					(*list)[entity->m_id] = entity;
				}
			}
		}

		if (nextLowerLeftCol >= 0 && nextLowerLeftRow < m_rows)
		{
			for (auto entity : m_zones[nextLowerLeftRow * m_cols + nextLowerLeftCol]->m_entities)
			{
				if (entity->m_id != id && entity->m_enabled && ut::manhattenDistance(position, entity->m_position) < distance)
				{
					(*list)[entity->m_id] = entity;
				}
			}
		}

		if (nextLowerRightCol < m_cols && nextLowerRightRow < m_rows)
		{
			for (auto entity : m_zones[nextLowerRightRow * m_cols + nextLowerRightCol]->m_entities)
			{
				if (entity->m_id != id && entity->m_enabled && ut::manhattenDistance(position, entity->m_position) < distance)
				{
					(*list)[entity->m_id] = entity;
				}
			}
		}
	}
}

std::vector<Zone*> Grid::allZonesWithin(ut::rectf rect)
{
	std::vector<Zone*> selectedZones;
	selectedZones.reserve(100);

	int firstCol = rect.x0 / m_size.x * m_cols;
	int lastCol = rect.x1 / m_size.x * m_cols;
	int firstRow = rect.y0 / m_size.y * m_rows;
	int lastRow = rect.y1 / m_size.y * m_rows;

	for (int col = std::max(0, firstCol); col <= lastCol && col < m_cols; col++)
	{
		for (int row = std::max(0, firstRow); row <= lastRow && row < m_rows; row++)
		{
			selectedZones.push_back(m_zones[row*m_cols + col]);
		}
	}

	return selectedZones;
}

std::vector<std::shared_ptr<Entity>> Grid::allEntitiesWithin(ut::rectf rect)
{
	std::vector<Zone*> selectedZones = allZonesWithin(rect);
	std::vector<std::shared_ptr<Entity>> selectedEntities;
	selectedEntities.reserve(2000);

	for (Zone* zone : selectedZones)
	{
		zone->entityAccess.lock();
		for (auto entity : zone->m_entities)
		{
			if (ut::pointInRect(rect, entity->m_position))
			{
				selectedEntities.push_back(entity);
			}
		}
		zone->entityAccess.unlock();
	}

	return selectedEntities;
}
