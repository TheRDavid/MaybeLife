#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Zone;
class Grid
{
public:

	Grid(int _numZones, int zoneCapacity, sf::Vector2i size);

	std::vector<Zone*> zones;
	int numZones;
	int cols;
	int rows;
	float zoneWidth, zoneHeight;
	sf::Vector2i size;

	bool legalPosition(sf::Vector2f position);
	Zone* zoneAt(sf::Vector2f position);
	std::string to_string();

private:

	std::vector<Zone*> neighbours(Zone* zone);
};

