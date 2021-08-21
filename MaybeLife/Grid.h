#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Zone;
class Grid
{
public:

	Grid(int _numZones, int zoneCapacity, sf::Vector2i size);

	std::vector<Zone*> m_zones;
	int m_numZones;
	int m_cols;
	int m_rows;
	float m_zoneWidth, m_zoneHeight;
	sf::Vector2i m_size;

	bool legalPosition(sf::Vector2f position);
	Zone* zoneAt(sf::Vector2f position);
	std::string to_string();

private:

	std::vector<Zone*> neighbours(Zone* zone);
};

