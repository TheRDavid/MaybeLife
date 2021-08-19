#include "Environment.h"
#include "AppConfig.h"
#include "Utilities.h"
#include <time.h>
#include <stdlib.h>
Environment::Environment(RenderWindow * renderWindow, Vector2i size, int _numZones, int threads)
{
	centerShape.setRadius(gravityShapeRadius);
	centerShapeSize = Vector2f(gravityShapeRadius / 2, gravityShapeRadius / 2);
	centerShape.setFillColor(Color(255, 255, 255, 128));
	centerShape.setOutlineColor(sf::Color::Red);
	centerShape.setOutlineThickness(2);
	centerShape.setPosition(10, 10);
	gravityCenter = Vector2f(size.x / 2, size.y / 2);
	numThreads = threads;
	steps = new int[max(threads, 1)];
	for (int i = 0; i < max(threads, 1); i++) {
		steps[i] = 0;
	}
	zoneRows = max(1, (int)sqrt(_numZones / (size.x / size.y)));
	zoneCols = _numZones / zoneRows;
	numZones = zoneCols * zoneRows;
	showZones = numZones <= 1000;
	zones.reserve(numZones);
	window = renderWindow;
	nextEntityId = 0;
	this->size = size;
	zoneWidth = size.x / (float)zoneCols;
	zoneHeight = size.y / (float)zoneRows;

	zoneLines = new VertexArray(sf::Lines, (zoneCols + zoneRows) * 2);
}

void Environment::start(vector<Entity*>* entities) {
	this->entities = entities;

	int zoneCapacity = 2 * entities->size() / numZones;
	cout << numZones << " Zones with " << zoneRows << " rows and " << zoneCols << " cols, each " << zoneWidth << "x" << zoneHeight << endl;
	for (int i = 0; i < numZones; i++) {
		int row = floor(i / zoneCols);
		int col = floor(i - row * zoneCols);
		float xPos = col * zoneWidth, yPos = row * zoneHeight;
		zones.push_back(new Zone(nextZoneId++, this, xPos, xPos + zoneWidth, yPos, yPos + zoneHeight, zoneCapacity));
	}
	for (int i = 0; i < numZones; i++) {
		Zone* zone = zones[i];
		zone->neighbours = neighbours(zone);
	}
	for (Entity* entity : (*entities)) {
		entity->environment = this;
		Zone* zone = zoneAt(entity->position);
		entity->zone = zone;
		zone->entities.push_back(entity);
	}

	for (int i = 0; i < numThreads; i++) {
		steps[i] = -1;
		int firstZone = i == 0 ? 0 : i * numZones / numThreads;
		int lastZone = i == 0 ? numZones / numThreads : (i + 1)*numZones / numThreads;
		cout << "New Thread: " << i << " zones " << firstZone << " to " << lastZone << endl;

		if (i == 0) {
			new thread(&Environment::updateEntities, this, 0, numZones / numThreads, 0);
		}
		else {
			new thread(&Environment::updateEntities, this, i * numZones / numThreads, (i + 1) * numZones / numThreads, i);

		}
	}
}

Zone* Environment::zoneAt(Vector2f position)
{
	if (!legalPosition(position)) {
		return nullptr;
	}
	int index = (int)(position.y / zoneHeight) * zoneCols + (int)(position.x / zoneWidth);
	return zones[index];
}

bool Environment::legalPosition(Vector2f position)
{
	return 0 <= position.x && position.x <= size.x && 0 <= position.y && position.y <= size.y;
}

vector<Zone*> Environment::neighbours(Zone* zone)
{
	vector<Zone*> neighbours;
	neighbours.push_back(zone);

	Zone* neighbour = zoneAt(Vector2f(zone->xMid - zoneWidth, zone->yMid));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(Vector2f(zone->xMid, zone->yMid - zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(Vector2f(zone->xMid, zone->yMid + zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(Vector2f(zone->xMid + zoneWidth, zone->yMid));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(Vector2f(zone->xMid + zoneWidth, zone->yMid + zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(Vector2f(zone->xMid - zoneWidth, zone->yMid - zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(Vector2f(zone->xMid + zoneWidth, zone->yMid - zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(Vector2f(zone->xMid - zoneWidth, zone->yMid + zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	return neighbours;
}


void Environment::updateEntities(int firstZone, int lastZone, int threadN)
{
	int stepIdx = max(0, threadN);

	while (true) {
		bool allReady = true;
		for (int i = 0; i < numThreads; i++) {
			if (steps[i] != steps[stepIdx])
				allReady = false;
		}
		if (!allReady) {
			for (int i = 0; i < numThreads; i++) {
				if (steps[stepIdx] < steps[i])
					allReady = true;
			}
		}
		if (allReady) {
			for (int i = firstZone; i < lastZone; i++)
			{
				zones[i]->update();
			}

			for (int i = firstZone; i < lastZone; i++)
			{
				Zone* uZone = zones[i];
				for (Entity* entity : uZone->entities)
				{
					entity->update();
				}
			}

			steps[stepIdx] = steps[stepIdx] + 1;
		}
		if (threadN == -1) break;
	}
}

void Environment::draw()
{
	if (rects == NULL) {
		rects = new VertexArray(sf::Quads, 4 * entities->size());
	}

	if (showZones)
		drawZones();

	rects->clear();
	int lineIndex = 0;
	int idx = 0;
	for (Entity* entity : *entities)
	{
		if (inRenderRect(entity)) {
			Vector2f ePos = entity->position;
			Vector2f eSize = entity->size;
			Color col = entity->color;
			rects->append(Vector2f(ePos.x - eSize.x, ePos.y - eSize.y));
			rects->append(Vector2f(ePos.x + eSize.x, ePos.y - eSize.y));
			rects->append(Vector2f(ePos.x + eSize.x, ePos.y + eSize.y));
			rects->append(Vector2f(ePos.x - eSize.x, ePos.y + eSize.y));
		}
	}


	window->draw(*rects);
	//zoneLines->clear();
	centerShape.setPosition(Vector2f(gravityCenter.x - gravityShapeRadius, gravityCenter.y - gravityShapeRadius));
	window->draw(centerShape);
	//connectionLines->clear();
}

string Environment::stepsToString()
{
	string str = "Steps: ";
	for (int i = 0; i < max(1, numThreads); i++) {
		str += " " + to_string(steps[i]);
	}
	return str;
}

void Environment::drawZones()
{

	if (selectedZone != nullptr) {
		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(selectedZone->xEnd - selectedZone->xStart, selectedZone->yEnd - selectedZone->yStart));
		rectangle.setOutlineColor(sf::Color::Red);
		rectangle.setOutlineThickness(1);
		rectangle.setFillColor(sf::Color(70, 70, 190, 100));
		rectangle.setPosition(selectedZone->xStart, selectedZone->yStart);
		window->draw(rectangle);
	}

	int ldx = 0;
	Color lC = Color(150, 150, 150, 255);
	for (int i = 0; i < zoneRows; i++) {
		(*zoneLines)[ldx].position = Vector2f(0, i * zoneHeight);
		(*zoneLines)[ldx++].color = lC;
		(*zoneLines)[ldx].position = Vector2f(size.x, i * zoneHeight);
		(*zoneLines)[ldx++].color = lC;
	}
	for (int i = 0; i < zoneCols; i++) {
		(*zoneLines)[ldx].position = Vector2f(i * zoneWidth, 0);
		(*zoneLines)[ldx++].color = lC;
		(*zoneLines)[ldx].position = Vector2f(i * zoneWidth, size.y);
		(*zoneLines)[ldx++].color = lC;
	}
	window->draw(*zoneLines);
}

bool Environment::inRenderRect(Entity * entity)
{
	//cout << entity->to_bounds_string() + " in " << ut::to_string(renderRectPosition) << " x " << ut::to_string(renderRectSize) << endl;
	return
		// x overlapping?
		(entity->position.x + entity->size.x > renderRectPosition.x
			&& entity->position.x - entity->size.x < renderRectPosition.x + renderRectSize.x)
		&&
		// y overlapping?
		(entity->position.y + entity->size.y > renderRectPosition.y
			&& entity->position.y - entity->size.y < renderRectPosition.y + renderRectSize.y);
}

bool Environment::inRenderRect(Zone * zone)
{
	return
		// x overlapping?
		(zone->xEnd > renderRectPosition.x && zone->xStart < renderRectPosition.x + renderRectSize.x)
		&&
		// y overlapping?
		(zone->yEnd > renderRectPosition.y && zone->yStart < renderRectPosition.y + renderRectSize.y);
}

