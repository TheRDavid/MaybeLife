#include "Grid.h"

#include <math.h>
#include <iostream>

#include "Zone.h"
#include "Utilities.h"

Grid::Grid(int _numZones, int zoneCapacity, sf::Vector2i size) {
	this->m_size = size;
	m_rows = std::max(1, (int)sqrt(_numZones / (m_size.x / m_size.y)));
	m_cols = _numZones / m_rows;
	m_numZones = m_cols * m_rows;
	m_zones.reserve(m_numZones);
	m_zoneWidth = m_size.x / (float)m_cols;
	m_zoneHeight = m_size.y / (float)m_rows;

	for (int i = 0; i < m_numZones; i++) {
		int row = floor(i / m_cols);
		int col = floor(i - row * m_cols);
		float xPos = col * m_zoneWidth, yPos = row * m_zoneHeight;
		m_zones.push_back(new Zone(this, xPos, xPos + m_zoneWidth, yPos, yPos + m_zoneHeight, zoneCapacity));
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