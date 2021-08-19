#include "Grid.h"

#include <math.h>
#include <iostream>

#include "Zone.h"
#include "Utilities.h"

Grid::Grid(int _numZones, int zoneCapacity, sf::Vector2i size) {
	this->size = size;
	rows = std::max(1, (int)sqrt(_numZones / (size.x / size.y)));
	cols = _numZones / rows;
	numZones = cols * rows;
	zones.reserve(numZones);
	zoneWidth = size.x / (float)cols;
	zoneHeight = size.y / (float)rows;

	for (int i = 0; i < numZones; i++) {
		int row = floor(i / cols);
		int col = floor(i - row * cols);
		float xPos = col * zoneWidth, yPos = row * zoneHeight;
		zones.push_back(new Zone(this, xPos, xPos + zoneWidth, yPos, yPos + zoneHeight, zoneCapacity));
	}
	for (int i = 0; i < numZones; i++) {
		Zone* zone = zones[i];
		zone->neighbours = neighbours(zone);
	}

	std::cout << "Created Grid: \n" << to_string() << std::endl;
}

std::vector<Zone*> Grid::neighbours(Zone* zone)
{
	std::vector<Zone*> neighbours;
	neighbours.push_back(zone);

	Zone* neighbour = zoneAt(sf::Vector2f(zone->xMid - zoneWidth, zone->yMid));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid, zone->yMid - zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid, zone->yMid + zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid + zoneWidth, zone->yMid));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid + zoneWidth, zone->yMid + zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid - zoneWidth, zone->yMid - zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid + zoneWidth, zone->yMid - zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(sf::Vector2f(zone->xMid - zoneWidth, zone->yMid + zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	return neighbours;
}

Zone* Grid::zoneAt(sf::Vector2f position)
{
	if (!legalPosition(position)) {
		return nullptr;
	}
	int index = (int)(position.y / zoneHeight) * cols + (int)(position.x / zoneWidth);
	return zones[index];
}

bool Grid::legalPosition(sf::Vector2f position)
{
	return 0 <= position.x && position.x <= size.x && 0 <= position.y && position.y <= size.y;
}

std::string Grid::to_string()
{
	return "Grid (" + std::to_string(numZones) + "), size=" + ut::to_string(size) + ", cols="
		+ std::to_string(cols) + ", rows=" + std::to_string(rows) + ", zone width=" + std::to_string(zoneWidth)
		+ ", zone height=" + std::to_string(zoneHeight);
}