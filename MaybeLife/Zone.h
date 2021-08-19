#pragma once
#include <mutex>
#include <SFML/Graphics.hpp>
class Entity;
class Grid;
class Zone {
public:
	Grid* grid;
	Zone(Grid* grid, float xStart, float xEnd, float yStart, float yEnd, int capacity);
	unsigned long long int id = ID();
	std::vector<Zone*> neighbours;
	float xStart, xEnd, yStart, yEnd, xMid, yMid;
	std::vector<Entity*> entities;
	void update();
	bool legalPosition(sf::Vector2f position);
	std::string toString();

	void addEntity(Entity* entity);
	void addEntityImmediatly(Entity * entity);

private:
	std::vector<Entity*> toRemove;
	std::vector<Entity*> toAdd;
	std::mutex entityAccess;

	static unsigned long long int ID()
	{
		static unsigned long long int ID = 0;
		return ID++;
	}
};