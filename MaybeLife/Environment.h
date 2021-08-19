#pragma once
#include <vector>;
#include <mutex>

#include "Zone.h"
#include "Entity.h"
class Environment
{

public:

	Environment(sf::RenderWindow* renderWindow, sf::Vector2i size, int numZones, int threads);
	Zone* zoneAt(sf::Vector2f position);
	std::vector<Zone*> neighbours(Zone* zone);
	std::vector<Zone*> zones;
	void updateEntities(int firstZone, int lastZone, int threadN);
	unsigned long long int nextEntityId, nextZoneId;
	sf::Vector2i size;
	sf::Vector2f gravityCenter;
	void draw();
	std::vector<Entity*>* entities;
	sf::RenderWindow* window;
	sf::VertexArray* rects = NULL;
	sf::VertexArray* zoneLines = NULL;
	int numZones, zoneRows, zoneCols;
	float zoneWidth, zoneHeight;
	int* steps;
	void start(std::vector<Entity*>* entities);
	std::string stepsToString();
	int numThreads;
	bool showZones = false, showUI = true, showLines, entityCollision = true;
	sf::CircleShape centerShape;
	sf::RectangleShape processedZoneRect;
	float gravityShapeRadius = 10;
	sf::Vector2f centerShapeSize;
	sf::Vector2f left = sf::Vector2f(-2, 0);
	sf::Vector2f upLeft = sf::Vector2f(-2, -2);
	sf::Vector2f up = sf::Vector2f(0, -2);
	sf::Vector2f upRight = sf::Vector2f(2, -2);
	sf::Vector2f right = sf::Vector2f(2, 0);
	sf::Vector2f downRight = sf::Vector2f(2, 2);
	sf::Vector2f down = sf::Vector2f(0, 2);
	sf::Vector2f downLeft = sf::Vector2f(-2, 2);
	sf::Vector2f gridDirections[8] = { left, upLeft, up, upRight, right, downRight, down, downLeft };
	bool legalPosition(sf::Vector2f position);
	std::mutex insertLock;

	sf::Vector2f renderRectPosition;
	sf::Vector2f renderRectSize;

	Zone* selectedZone = nullptr;
	Entity* selectedEntity = nullptr;

private:
	int processedZone = 0;
	void drawZones();
	sf::Color emptyZoneColor = sf::Color(30, 30, 90, 128);
	bool inRenderRect(Entity* entity);
	bool inRenderRect(Zone* zone);
};


