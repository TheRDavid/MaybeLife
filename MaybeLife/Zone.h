#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;
class Entity;
class Environment;
class Zone {
public:
	Zone(unsigned long long int id, Environment* environment, float xStart, float xEnd, float yStart, float yEnd, int capacity);
	unsigned long long int id;
	std::vector<Zone*> neighbours;
	float xStart, xEnd, yStart, yEnd, xMid, yMid;
	vector<Entity*> entities;
	Environment* environment;
	void update();
	bool legalPosition(Vector2f position);
	std::string toString();
private:
	vector<Entity*> toRemove;
};