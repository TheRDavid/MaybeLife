#pragma once
#include <mutex>
#include <SFML/Graphics.hpp>
class Entity;
class Grid;
class Zone {
public:
	Zone(Grid* grid, float xStart, float xEnd, float yStart, float yEnd, int capacity);
	unsigned long long int id = ID();
	std::vector<Zone*> neighbours;
	float xStart, xEnd, yStart, yEnd, xMid, yMid;
	std::vector<Entity*> entities;

	void update();
	std::string to_string();

	void addEntity(Entity* entity);
	void addEntityImmediatly(Entity * entity);

private:
	Grid* grid;
	std::vector<Entity*> toRemove;
	std::vector<Entity*> toAdd;
	std::mutex entityAccess;

	bool legalPosition(sf::Vector2f position);

	static unsigned long long int ID()
	{
		static unsigned long long int ID = 0;
		return ID++;
	}
};