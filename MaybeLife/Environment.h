#pragma once
#include <vector>;
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
#include "Zone.h"
using namespace sf;
class Environment
{

public:

	Environment(RenderWindow* renderWindow, Vector2i size, int numZones, int threads);
	Zone* zoneAt(Vector2f position);
	vector<Zone*> neighbours(Zone* zone);
	vector<Zone*> zones;
	void updateEntities(int firstZone, int lastZone, int threadN);
	unsigned long long int nextEntityId, nextZoneId;
	Vector2i size;
	Vector2f gravityCenter;
	void draw();
	vector<Entity*>* entities;
	RenderWindow* window;
	VertexArray* rects = NULL;
	VertexArray* connectionLines = NULL;
	VertexArray* zoneLines = NULL;
	int numZones, zoneRows, zoneCols;
	float zoneWidth, zoneHeight;
	int* steps;
	void setMaximumNumberOfLines(int newMaxLines);
	void start(vector<Entity*>* entities);
	string stepsToString();
	int numThreads;
	bool showZones = false, showUI = true, showLines, entityCollision = true;
	sf::CircleShape centerShape;
	sf::RectangleShape processedZoneRect;
	float gravityShapeRadius = 10;
	Vector2f centerShapeSize;
	Vector2f left = Vector2f(-2, 0);
	Vector2f upLeft = Vector2f(-2, -2);
	Vector2f up = Vector2f(0, -2);
	Vector2f upRight = Vector2f(2, -2);
	Vector2f right = Vector2f(2, 0);
	Vector2f downRight = Vector2f(2, 2);
	Vector2f down = Vector2f(0, 2);
	Vector2f downLeft = Vector2f(-2, 2);
	Vector2f gridDirections[8] = { left, upLeft, up, upRight, right, downRight, down, downLeft };
	bool legalPosition(Vector2f position);
	std::mutex insertLock;
private:
	int processedZone = 0;
	void drawZones();
	Color emptyZoneColor = Color(30, 30, 90, 128);
	int maxLines = 2000000;


};


