#pragma once
#include <vector>;
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "Entities.h"
#include <mutex>
using namespace std;
#include "Zone.h"
using namespace sf;
class Environment
{

public:

	enum Behaviour {
		SPREAD, RANDOM, GRAVITATE, SLEEP, FALL
	};

	Environment(RenderWindow* renderWindow, Vector2i size, int numZones, int threads, vector<Entity*>* entities, Environment::Behaviour behaviour);
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
	bool legalPosition(Vector2f position);
	bool legalPosition_strict(Entity* entity, Vector2f position, Zone* zone);
	VertexArray* rects = NULL;
	VertexArray* connectionLines = NULL;
	VertexArray* zoneLines = NULL;
	int numZones, zoneRows, zoneCols;
	float zoneWidth, zoneHeight;
	int* steps;
	void setMaximumNumberOfLines(int newMaxLines);
	string stepsToString();
	int numThreads;
	Behaviour behaviour;
	bool showZones = false, showUI = true, showLines, entityCollision = true;
	sf::CircleShape centerShape;
	sf::RectangleShape processedZoneRect;
	float gravityShapeRadius = 10;
	Vector2f centerShapeSize;
private:
	int processedZone = 0;
	void drawZones();
	Color emptyZoneColor = Color(30, 30, 90, 128);
	void entitiesDoGravitate(int firstZone, int lastZone, int threadN);
	void entitiesDoRandom(int firstZone, int lastZone, int threadN);
	void entitiesDoSpread(int firstZone, int lastZone, int threadN);
	void entitiesDoFall(int firstZone, int lastZone, int threadN);
	int maxLines = 200000;

	bool colliding(Entity* entity, Vector2f position, Zone* zone);
	bool colliding(Entity* entity, Zone* zone);

	Vector2f left = Vector2f(-2, 0);
	Vector2f upLeft = Vector2f(-2, -2);
	Vector2f up = Vector2f(0, -2);
	Vector2f upRight = Vector2f(2, -2);
	Vector2f right = Vector2f(2, 0);
	Vector2f downRight = Vector2f(2, 2);
	Vector2f down = Vector2f(0, 2);
	Vector2f downLeft = Vector2f(-2, 2);
	Vector2f gridDirections[8] = {left, upLeft, up, upRight, right, downRight, down, downLeft};
};


