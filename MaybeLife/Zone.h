#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
using namespace sf;
class Entity;
class Zone {
public:
	Zone();
	Zone(unsigned long long int id, float xStart, float xEnd, float yStart, float yEnd);
	unsigned long long int id;
	std::vector<Zone*> neighbours;
	float xStart, xEnd, yStart, yEnd, xMid, yMid;
	std::unordered_map<int, Entity*> entities;
	bool legalPosition(Vector2f position);
	std::string toString();
};