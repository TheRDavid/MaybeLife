#include "Environment.h"
#include "AppConfig.h"

Environment::Environment(RenderWindow * renderWindow, Vector2i size, int _numZones, int threads, vector<Entity*>* entities, Environment::Behaviour behaviour)
{
	this->behaviour = behaviour;
	this->entities = entities;
	numThreads = threads;
	steps = new int[max(threads, 1)];
	for (int i = 0; i < max(threads, 1); i++) {
		steps[i] = 0;
	}
	zoneRows = max(1,(int)sqrt(_numZones / (size.x / size.y)));
	zoneCols = _numZones / zoneRows;
	numZones = zoneCols * zoneRows;
	int zoneCapacity = 2 * entities->size() / numZones;
	zones.reserve(numZones);
	locks.reserve(numZones);
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
		//cout << "New zone at " << row << "," << col << " cap=" << zoneCapacity << " zone has " << zones.size() << endl;
		//zones.push_back(new Zone(nextZoneId++, xPos, xPos + zoneWidth, yPos, yPos + zoneHeight, zoneCapacity));
		zones[i] = new Zone(nextZoneId++, xPos, xPos + zoneWidth, yPos, yPos + zoneHeight, zoneCapacity);
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
	switch (behaviour) {
	case Behaviour::RANDOM:
		entitiesDoRandom(firstZone, lastZone, threadN);
		break;
	case Behaviour::SPREAD:
		entitiesDoSpread(firstZone, lastZone, threadN);
		break;
	default:break;
	}
}

float RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}
void Environment::entitiesDoRandom(int firstZone, int lastZone, int threadN)
{
		if (lines == NULL) {
			lines = new VertexArray(sf::Lines, zoneCols * zoneRows);
		}
	int idx = numZones > 256 ? 0 : 256;
	int stepIdx = max(0, threadN);
	//cout << "Inside Thread!" << endl;
	vector<Entity*> toRemove;
	toRemove.reserve(500000);
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
			//cout << "Step 0" << endl;
			steps[stepIdx] = steps[stepIdx] + 1;
			for (int i = firstZone; i < lastZone; i++)
			{
				toRemove.clear();
				Zone* zone = zones[i];
				for (Entity* entity : zone->entities) {
					if (!zone->legalPosition(entity->position)) {
						toRemove.push_back(entity);
						Zone* newZone = zoneAt(entity->position);
						if (newZone == nullptr) {
							cout << "Illegal Entity: " << entity->toString() << endl;
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
			//cout << "Step 1" << endl;blue
			for (int i = firstZone; i < lastZone; i++)
			{
				//	cout << "Step 1.1" << endl;
				Zone* uZone = zones[i];
				for (Entity* entity : uZone->entities)
				{
					//	cout << "Step 1.2" << endl;
					Vector2f dir = Vector2f(RandomNumber(-1,1), RandomNumber(-1, 1));
					Vector2f ePos = entity->position + dir;

					if (legalPosition_strict(ePos)) {
						entity->position = ePos;
					}
				}
			}
		}
		//cout << "Step 2" << endl;
		if (threadN == -1) break;
	}
}
void Environment::entitiesDoSpread(int firstZone, int lastZone, int threadN)
{
	bool drawLines = entities->size() <= 1000;

	if (drawLines) {
		if (lines == NULL) {
			lines = new VertexArray(sf::Lines, entities->size()*entities->size() * 2);
		}
	}
	else
		if (lines == NULL) {
			lines = new VertexArray(sf::Lines, zoneCols * zoneRows);
		}
	int idx = numZones > 256 ? 0 : 256;
	int stepIdx = max(0, threadN);
	//cout << "Inside Thread!" << endl;
	vector<Entity*> toRemove;
	toRemove.reserve(100);
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
			//cout << "Step 0" << endl;
			steps[stepIdx] = steps[stepIdx] + 1;
			for (int i = firstZone; i < lastZone; i++)
			{
				toRemove.clear();
				Zone* zone = zones[i];
				for (Entity* entity : zone->entities) {
					if (!zone->legalPosition(entity->position)) {
						toRemove.push_back(entity);
						Zone* newZone = zoneAt(entity->position);
						if (newZone == nullptr) {
							cout << "Illegal Entity: " << entity->toString() << endl;
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
			//cout << "Step 1" << endl;blue
			for (int i = firstZone; i < lastZone; i++)
			{
				//	cout << "Step 1.1" << endl;
				Zone* uZone = zones[i];
				for (Entity* entity : uZone->entities)
				{
					//	cout << "Step 1.2" << endl;
					float xDir = 0, yDir = 0;
					Vector2f ePos = entity->position;
					Vector2f newPos = ePos;
					for (Zone* zone : uZone->neighbours) {
						//	cout << "Step 1.3" << endl;
						for (Entity* neighbour : zone->entities)
						{
							//	cout << "Step 1.4" << endl;
							if (entity->id != neighbour->id) {
								if (drawLines) {
									(*lines).append(Vector2f(ePos.x, ePos.y));
									(*lines).append(Vector2f(neighbour->position.x, neighbour->position.y));
								}
								float dist = pow(ePos.x - neighbour->position.x, 2) + pow(ePos.y - neighbour->position.y, 2);
								float xd = ePos.x - neighbour->position.x;
								if (xd != 0)
									xd = (xd > 0 ? 10 : -10) / dist;
								float yd = ePos.y - neighbour->position.y;
								if (yd != 0)
									yd = (yd > 0 ? 10 : -10) / dist;
								xDir += xd;
								yDir += yd;
							}
						}
					}
					//cout << "Step 1.5" << endl;
					if (xDir != 0) xDir = xDir > 0 ? 1 : -1;
					if (yDir != 0) yDir = yDir > 0 ? 1 : -1;
					newPos = ePos + Vector2f(xDir, yDir);
					if (legalPosition_strict(newPos)) {
						entity->position = newPos;
					}
				}
			}
		}
		//cout << "Step 2" << endl;
		if (threadN == -1) break;
	}
}

void Environment::draw()
{
	bool drawLines = entities->size() <= 1000;
	if (rects == NULL) {
		rects = new VertexArray(sf::Quads, 4 * entities->size());
	}

	if (numZones <= 10000)
		drawZones();
	int innerCount = 0;
	int idx = 0, lidx = 0;

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
	if (drawLines);
	window->draw(*lines);
	window->draw(*rects);
	lines->clear();
}

bool Environment::legalPosition_strict(Vector2f position)
{
	return 0 < position.x && position.x < size.x && 0 < position.y && position.y < size.y;
}

bool Environment::legalPosition(Vector2f position)
{
	return 0 <= position.x && position.x <= size.x && 0 <= position.y && position.y <= size.y;
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
	int idx = 0, ldx = 0;
	Text countText;
	countText.setFont(AppConfig::getInstance().mainFont);
	countText.setCharacterSize(16);
	countText.setFillColor(Color(90,90,90,90));
	countText.setStyle(Text::Bold);
	Color lC = Color(90, 90, 90, 90);
	for (int i = 0; i < numZones; i++)
	{
		Zone* zone = zones[i];
		countText.setPosition(Vector2f(zone->xStart, zone->yStart));
		countText.setString(to_string(zone->id) + ": #" + to_string(zone->entities.size()));
		window->draw(countText);
	}
	for (int i = 0; i < zoneRows; i++) {
		(*lines)[ldx++].color = lC;
		lines->append(Vector2f(0, i * zoneHeight));
		(*lines)[ldx++].color = lC;
		lines->append(Vector2f(size.x, i * zoneHeight));
	}
	for (int i = 0; i < zoneCols; i++) {
		(*lines)[ldx++].color = lC;
		lines->append(Vector2f(i * zoneWidth, 0));
		(*lines)[ldx++].color = lC;
		lines->append(Vector2f(i * zoneWidth, size.y));
	}
}
