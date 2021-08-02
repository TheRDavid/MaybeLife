#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;
class Entity;
class Zone {
public:
	Zone();
	Zone(unsigned long long int id, float xStart, float xEnd, float yStart, float yEnd, int capacity);
	unsigned long long int id;
	std::vector<Zone*> neighbours;
	float xStart, xEnd, yStart, yEnd, xMid, yMid;
	vector<int> entities;
	bool legalPosition(Vector2f position);
	std::string toString();
};