#pragma once
#include <vector>;
#include <mutex>

#include "Zone.h"
#include "Entity.h"
class Grid;
class Environment
{

public:
	Grid* entityGrid;
	std::vector<Entity*>* entities;
	sf::RenderWindow* window;

	sf::VertexArray* rects = NULL;
	sf::VertexArray* zoneLines = NULL;

	int numThreads;
	float gravityShapeRadius = 10;

	bool showZones = false;
	bool showUI = true;
	bool entityCollision = true;

	sf::CircleShape centerShape;
	sf::RectangleShape processedZoneRect;

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

	std::mutex insertLock;

	sf::Vector2i size;
	sf::Vector2f renderRectPosition;
	sf::Vector2f renderRectSize;
	sf::Vector2f gravityCenter;

	Zone* selectedZone = nullptr;
	Entity* selectedEntity = nullptr;

	Environment(sf::RenderWindow* renderWindow, sf::Vector2i size, int numZones, int threads, sf::View* sceneView);
	void updateEntities(int firstZone, int lastZone, int threadN);
	void draw();
	int* steps;
	void start(std::vector<Entity*>* entities);
	std::string stepsToString();
	bool legalPosition(sf::Vector2f position);

private:
	sf::View* sceneView;

	void drawZones();
	bool inRenderRect(Entity* entity);
	bool inRenderRect(Zone* zone);
};


