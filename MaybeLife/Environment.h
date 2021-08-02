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
		SPREAD, RANDOM
	};

	Environment(RenderWindow* renderWindow, Vector2i size, int numZones, int threads, vector<Entity*>* entities, Environment::Behaviour behaviour);
	Zone* zoneAt(Vector2f position);
	vector<Zone*> neighbours(Zone* zone);
	vector<Zone*> zones;
	void updateEntities(int firstZone, int lastZone, int threadN);
	unsigned long long int nextEntityId, nextZoneId;
	Vector2i size;
	void draw();
	vector<Entity*>* entities;
	RenderWindow* window;
	bool legalPosition(Vector2f position);
	bool legalPosition_strict(Vector2f position);
	VertexArray* rects = NULL;
	VertexArray* lines = NULL;
	int numZones, zoneRows, zoneCols;
	float zoneWidth, zoneHeight;
	int* steps;
	string stepsToString();
private:
	void drawZones();
	Color emptyZoneColor = Color(30, 30, 90, 128);
	int numThreads;
	vector<mutex*> locks;
	Behaviour behaviour;
	void entitiesDoSpread(int firstZone, int lastZone, int threadN);
	void entitiesDoRandom(int firstZone, int lastZone, int threadN);
};


