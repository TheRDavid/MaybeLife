#pragma once
#include <vector>;
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
#include "Zone.h"
using namespace sf;
class Environment
{
	
public:
	Environment(RenderWindow* renderWindow, Vector2i size, int numZones);
	void addEntity(Entity* entity);
	Zone* zoneAt(Vector2f position);
	vector<Zone*> neighbours(Zone* zone);
	map<int, Zone*> zones;
	void update();
	unsigned long long int nextEntityId, nextZoneId;
	Vector2i size;
	void draw(Vector2i upperLeft, Vector2i lowerRight);
	map<int, Entity*> entities;
	RenderWindow* window;
	bool legalPosition(Vector2f position);
	VertexArray* rects = NULL;
	VertexArray* lines = NULL;
	int sqrtNumZones, numZones;
	float zoneWidth, zoneHeight;
private:
	void drawZones();
	Color emptyZoneColor = Color(30, 30, 90, 128);
	vector<Entity*> toRemove;
};


