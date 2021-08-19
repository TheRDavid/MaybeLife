#pragma once
#include <mutex>
#include <SFML/Graphics.hpp>
class Entity;
class Environment;
class Zone {
public:
	Zone(unsigned long long int id, Environment* environment, float xStart, float xEnd, float yStart, float yEnd, int capacity);
	unsigned long long int id;
	std::vector<Zone*> neighbours;
	float xStart, xEnd, yStart, yEnd, xMid, yMid;
	std::vector<Entity*> entities;
	Environment* environment;
	void update();
	bool legalPosition(sf::Vector2f position);
	std::string toString();

	void addEntity(Entity* entity);

private:
	std::vector<Entity*> toRemove;
	std::vector<Entity*> toAdd;
	std::mutex entityAccess;
};