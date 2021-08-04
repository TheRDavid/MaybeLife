#include "Environment.h"
#include "AppConfig.h"
#include "Utilities.h"
#include <time.h>
#include <stdlib.h>
Environment::Environment(RenderWindow * renderWindow, Vector2i size, int _numZones, int threads, vector<Entity*>* entities, Environment::Behaviour behaviour)
{
	centerShape.setRadius(gravityShapeRadius);
	centerShapeSize = Vector2f(gravityShapeRadius / 2, gravityShapeRadius / 2);
	centerShape.setFillColor(Color(255,255,255,128));
	centerShape.setOutlineColor(sf::Color::Red);
	centerShape.setOutlineThickness(2);
	centerShape.setPosition(10, 10);
	gravityCenter = Vector2f(size.x / 2, size.y / 2);
	this->behaviour = behaviour;
	this->entities = entities;
	numThreads = threads;
	steps = new int[max(threads, 1)];
	for (int i = 0; i < max(threads, 1); i++) {
		steps[i] = 0;
	}
	zoneRows = max(1, (int)sqrt(_numZones / (size.x / size.y)));
	zoneCols = _numZones / zoneRows;
	numZones = zoneCols * zoneRows;
	showZones = numZones <= 1000;
	int zoneCapacity = 2 * entities->size() / numZones;
	zones.reserve(numZones);
	window = renderWindow;
	nextEntityId = 0;
	this->size = size;
	zoneWidth = size.x / (float)zoneCols;
	zoneHeight = size.y / (float)zoneRows;
	cout << numZones << " Zones with " << zoneRows << " rows and " << zoneCols << " cols, each " << zoneWidth << "x" << zoneHeight << endl;
	//cout << "At a size of " << renderWindow->getSize().x << "x" << renderWindow->getSize().y << " / " << size.x << "x" << size.y << " with " << sqrtNumZones << "^2 zones, each zone has size: " << zoneWidth << "x" << zoneHeight << endl;
	for (int i = 0; i < numZones; i++) {
		int row = floor(i / zoneCols);
		int col = floor(i - row * zoneCols);
		float xPos = col * zoneWidth, yPos = row * zoneHeight;
		zones.push_back(new Zone(nextZoneId++, xPos, xPos + zoneWidth, yPos, yPos + zoneHeight, zoneCapacity));
		//	zones[i] = new Zone(nextZoneId++, xPos, xPos + zoneWidth, yPos, yPos + zoneHeight, zoneCapacity);
			//zones[i] = new Zone(nextZoneId++, xPos, xPos + zoneWidth, yPos, yPos + zoneHeight, zoneCapacity);
	}
	for (int i = 0; i < numZones; i++) {
		Zone* zone = zones[i];
		zone->neighbours = neighbours(zone);
	}

	for (Entity* entity : (*entities)) {
		Zone* zone = zoneAt(entity->position);
		entity->zone = zone;
		zone->entities.push_back(entity);
	}
	for (int i = 0; i < numThreads; i++) {
		steps[i] = 0;
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

	connectionLines = new VertexArray(sf::Lines, maxLines);
	zoneLines = new VertexArray(sf::Lines, (zoneCols + zoneRows) * 2);
}

Zone* Environment::zoneAt(Vector2f position)
{
	if (!legalPosition(position)) {
		return nullptr;
	}
	int index = (int)(position.y / zoneHeight) * zoneCols + (int)(position.x / zoneWidth);
	return zones[index];
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

			vector<Entity*> toRemove;
			toRemove.reserve(100);
			for (int i = firstZone; i < lastZone; i++)
			{
				toRemove.clear();
				Zone* zone = zones[i];
				for (Entity* entity : zone->entities) {
					if (!zone->legalPosition(entity->position)) {
						toRemove.push_back(entity);
						Zone* newZone = zoneAt(entity->position);
						if (newZone == nullptr) {
							cout << "Illegal Entity: " << entity->to_string() << endl;
						}
						else {
							newZone->entities.push_back(entity);
							entity->zone = newZone;
						}
					}
				}
				for (auto &entity : toRemove) {
					zone->entities.erase(find(zone->entities.begin(), zone->entities.end(), entity));
				}
			}

			steps[stepIdx] = steps[stepIdx] + 1;
			switch (behaviour) {
			case Behaviour::GRAVITATE:
				entitiesDoGravitate(firstZone, lastZone, threadN);
				break;
			case Behaviour::RANDOM:
				entitiesDoRandom(firstZone, lastZone, threadN);
				break;
			case Behaviour::SPREAD:
				entitiesDoSpread(firstZone, lastZone, threadN);
				break;
			case Behaviour::FALL:
				entitiesDoFall(firstZone, lastZone, threadN);
				break;
			default:break;
			}
			if (entityCollision) {
				for (int i = firstZone; i < lastZone; i++)
				{
					Zone* zone = zones[i];
					for (Entity* entity : zone->entities) {
						srand(time(NULL));
						if (colliding(entity, entity->position, zone)) {
							bool foundDodge = false;
							Vector2f dodgePosition;
							int startDir = rand() % 8, dirCount = 0;
							while (dirCount++ < 8) {
								if (++startDir == 8) {
									startDir = 0;
								}
								dodgePosition = Vector2f(gridDirections[startDir].x * entity->size.x, gridDirections[startDir].y * entity->size.y) + entity->position;
								if (legalPosition_strict(entity, dodgePosition, zone)) {
									foundDodge = true;
									break;
								}
							}
							if (foundDodge) {
								entity->position = dodgePosition;
							}
						}
					}
				}
			}
		}
		if (threadN == -1) break;
	}
}

float RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}
void Environment::entitiesDoRandom(int firstZone, int lastZone, int threadN)
{
	for (int i = firstZone; i < lastZone; i++)
	{
		Zone* uZone = zones[i];
		for (Entity* entity : uZone->entities)
		{
			Vector2f dir = Vector2f(RandomNumber(-1, 1), RandomNumber(-1, 1));
			Vector2f ePos = entity->position + dir;

			if (legalPosition_strict(entity, ePos, uZone)) {
				entity->position = ePos;
			}
		}
	}
}
void Environment::entitiesDoFall(int firstZone, int lastZone, int threadN)
{
	for (int i = firstZone; i < lastZone; i++)
	{
		Zone* uZone = zones[i];
		for (Entity* entity : uZone->entities)
		{
			Vector2f dir = Vector2f(0, entity->size.y);
			Vector2f ePos = entity->position + dir;

			if (legalPosition_strict(entity, ePos, uZone)) {
				entity->position = ePos;
			}
			else {
				srand(time(NULL));
				int randDir = rand() == 1 ? 1 : -1;
				dir = Vector2f(randDir * entity->majorSize*2, entity->size.y);
				ePos = entity->position + dir;

				if (legalPosition_strict(entity, ePos, uZone)) {
					entity->position = ePos;
				}
				else {
					dir = Vector2f(-randDir * entity->majorSize*2, entity->size.y);
					ePos = entity->position + dir;

					if (legalPosition_strict(entity, ePos, uZone)) {
						entity->position = ePos;
					}
				}
			}
		}
	}
}
void Environment::entitiesDoGravitate(int firstZone, int lastZone, int threadN)
{
	for (int i = firstZone; i < lastZone; i++)
	{
		Zone* uZone = zones[i];
		for (Entity* entity : uZone->entities)
		{
			Vector2f dir = Vector2f(gravityCenter.x - entity->position.x, gravityCenter.y - entity->position.y);
			if (dir.x != 0) dir.x = dir.x > 0 ? 1 : -1;
			if (dir.y != 0) dir.y = dir.y > 0 ? 1 : -1;
			Vector2f newPos = entity->position + dir;
			if (legalPosition_strict(entity, newPos, uZone)) {
				entity->position = newPos;
			}
		}
	}
}

void Environment::entitiesDoSpread(int firstZone, int lastZone, int threadN)
{
	int lineIndex = 0;
	for (int i = firstZone; i < lastZone; i++)
	{
		Zone* uZone = zones[i];
		for (Entity* entity : uZone->entities)
		{
			float xDir = 0, yDir = 0;
			Vector2f ePos = entity->position;
			Vector2f newPos = ePos;
			for (Zone* zone : uZone->neighbours) {
				for (Entity* neighbour : zone->entities)
				{
					float xd = ePos.x - neighbour->position.x;
					float yd = ePos.y - neighbour->position.y;
					float dist = pow(xd, 2) + pow(yd, 2);
					if (entity->id != neighbour->id) {
						if (xd != 0)
							xd = (xd > 0 ? 1000 : -1000) / dist;
						if (yd != 0)
							yd = (yd > 0 ? 1000 : -1000) / dist;
						xDir += xd / dist;
						yDir += yd / dist;
						/*
												float xdi = ePos.x - neighbour->position.x;	// 0.4
												float ydi = ePos.y - neighbour->position.y; // -2.1

												float axdi = fabs(xdi);						// 0.4
												float aydi = fabs(ydi);						// 2.1

												float xdiWeight = axdi / aydi;				// 0.19
												float ydiWeight = aydi / axdi;				// 5.25

												if (xdiWeight > ydiWeight) {
													xdiWeight = 1;							// 0.19
												}
												else {
													ydiWeight = 1;							// 1
												}

												float xdir = (xdi > 0 ? 1 : -1); // 0.190
												float ydir = (ydi > 0 ? 1 : -1); // -1

												xDir += xdi == 0 ? 0 : xdir;
												yDir += ydi == 0 ? 0 : ydir;*/
					}
				}
			}
			if (xDir != 0) xDir = xDir > 0 ? 1 : -1;
			if (yDir != 0) yDir = yDir > 0 ? 1 : -1;
			newPos = ePos + Vector2f(xDir, yDir);
			if (legalPosition_strict(entity, newPos, uZone)) {
				entity->position = newPos;
			}
			else {
				newPos = ePos + Vector2f(0, yDir);
				if (legalPosition_strict(entity, newPos, uZone)) {
					entity->position = newPos;
				}
			}
		}
	}
}

bool Environment::colliding(Entity* entity, Zone * zone)
{
	return colliding(entity, entity->position, zone);
}

bool Environment::colliding(Entity* entity, Vector2f position, Zone * zone)
{
	for (Zone* z : zone->neighbours) {
		for (Entity* otherEntity : z->entities) {
			if (entity->id != otherEntity->id
				&& fabs(position.x - otherEntity->position.x) < (entity->size.x + otherEntity->size.x)
				&& fabs(position.y - otherEntity->position.y) < (entity->size.y + otherEntity->size.y)
				)
			{
				return true;
			}
		}
	}
	return false;
}

void Environment::draw()
{
	if (rects == NULL) {
		rects = new VertexArray(sf::Quads, 4 * entities->size());
	}

	if (showZones)
		drawZones();

	int lineIndex = 0;
	if (showLines) {
		for (int i = 0; i < numZones; i++)
		{
			Zone* uZone = zones[i];
			for (Entity* entity : uZone->entities)
			{
				for (Zone* zone : uZone->neighbours) {
					for (Entity* neighbour : zone->entities)
					{
						if (lineIndex < (maxLines)) {
							(*connectionLines)[lineIndex].color = entity->color;
							(*connectionLines)[lineIndex++].position = entity->position;
						}
						if (lineIndex < (maxLines)) {
							(*connectionLines)[lineIndex].color = neighbour->color;
							(*connectionLines)[lineIndex++].position = neighbour->position;
						}
					}
				}
			}
		}
	}
	int idx = 0;

	for (Entity* entity : (*entities))
	{
		Vector2f ePos = entity->position;
		Vector2f eSize = entity->size;
		Color col = entity->color;
		(*rects)[idx].color = col;
		(*rects)[idx++].position = Vector2f(ePos.x - eSize.x, ePos.y - eSize.y);
		(*rects)[idx].color = col;
		(*rects)[idx++].position = Vector2f(ePos.x + eSize.x, ePos.y - eSize.y);
		(*rects)[idx].color = col;
		(*rects)[idx++].position = Vector2f(ePos.x + eSize.x, ePos.y + eSize.y);
		(*rects)[idx].color = col;
		(*rects)[idx++].position = Vector2f(ePos.x - eSize.x, ePos.y + eSize.y);
	}



	if (showLines)
		window->draw(*connectionLines);
	window->draw(*rects);
	//zoneLines->clear();
	centerShape.setPosition(Vector2f(gravityCenter.x - gravityShapeRadius, gravityCenter.y - gravityShapeRadius));
	window->draw(centerShape);
	//connectionLines->clear();
}

bool Environment::legalPosition_strict(Entity* entity, Vector2f position, Zone* uZone)
{
	if (entityCollision && colliding(entity, position, uZone)) {
		return false;
	}
	return 0 < position.x && position.x < size.x && 0 < position.y && position.y < size.y;
}

bool Environment::legalPosition(Vector2f position)
{
	return 0 <= position.x && position.x <= size.x && 0 <= position.y && position.y <= size.y;
}

void Environment::setMaximumNumberOfLines(int newMaxLines)
{
	maxLines = newMaxLines;
	connectionLines->clear();
	delete(connectionLines);
	connectionLines = new VertexArray(sf::Lines, maxLines);
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
	int ldx = 0;
	Text countText;
	countText.setFont(AppConfig::getInstance().mainFont);
	countText.setCharacterSize(16);
	countText.setFillColor(Color(90, 90, 90, 180));
	countText.setStyle(Text::Bold);
	Color lC = Color(150, 150, 150, 255);
	for (int i = 0; i < numZones; i++)
	{
		Zone zone = *zones[i];
		countText.setPosition(Vector2f(zone.xStart, zone.yStart));
		countText.setString(to_string(zone.id) + ": #" + to_string(zone.entities.size()));
		window->draw(countText);
	}
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
