#pragma once
#include <mutex>
#include <SFML/Graphics.hpp>
class Entity;
class Grid;
class Zone {
public:
	Zone(Grid* grid, float xStart, float xEnd, float yStart, float yEnd, int capacity, int row, int col);
	unsigned long long int m_id = ID();
	std::vector<Zone*> m_neighbours;
	int m_row, m_col;
	float xStart, xEnd, yStart, yEnd, xMid, yMid;
	std::vector<std::shared_ptr<Entity>> m_entities;

	void update();
	std::string to_string();

	void addEntity(std::shared_ptr<Entity> entity);
	void removeEntity(std::shared_ptr<Entity> entity);
	void addEntityImmediatly(std::shared_ptr<Entity> entity);
	void removeEntityImmediatly(std::shared_ptr<Entity> entity);
	std::mutex entityAccess;
private:
	Grid* grid;
	std::vector<std::shared_ptr<Entity>> toRemove;
	std::vector<std::shared_ptr<Entity>> toAdd;

	bool legalPosition(sf::Vector2f position);

	static unsigned long long int ID()
	{
		static unsigned long long int ID = 0;
		return ID++;
	}
};