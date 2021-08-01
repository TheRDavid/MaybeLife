#include "Environment.h"
#include "AppConfig.h"

Environment::Environment(RenderWindow * renderWindow, Vector2i size, int sqrtNumZones)
{
	this->sqrtNumZones = sqrtNumZones;
	numZones = sqrtNumZones * sqrtNumZones;
	window = renderWindow;
	nextEntityId = 0;
	this->size = size;
	zoneWidth = size.x / (float)sqrtNumZones;
	zoneHeight = size.y / (float)sqrtNumZones;
	//cout << "At a size of " << renderWindow->getSize().x << "x" << renderWindow->getSize().y << " / " << size.x << "x" << size.y << " with " << sqrtNumZones << "^2 zones, each zone has size: " << zoneWidth << "x" << zoneHeight << endl;
	for (int i = 0; i < numZones; i++) {
		int row = floor(i / sqrtNumZones);
		int col = floor(i - row * sqrtNumZones);
		float xPos = col * zoneWidth, yPos = row * zoneHeight;
		zones[i] = new Zone(nextZoneId++, xPos, xPos + zoneWidth, yPos, yPos + zoneHeight);
	}
	for (int i = 0; i < numZones; i++) {
		Zone* zone = zones[i];
		zone->neighbours = neighbours(zone);
	}
	toRemove.reserve(100);
}

void Environment::addEntity(Entity* entity)
{
	entities[entity->id] = entity;
	Zone* zone = zoneAt(entity->position);
	if (zone == nullptr) {
		cout << "Illegal Entity: " << entity->toString() << endl;
		entity->color = Color::Red;
	}
	else {
		zone->entities[entity->id] = entity;
		entity->zone = zone;
	}
}

Zone* Environment::zoneAt(Vector2f position)
{
	if (position.x > size.x || position.x < 0 || position.y > size.y || position.y < 0) {
		return nullptr;
	}
	int row = position.y / zoneHeight;
	int col = position.x / zoneWidth;
	int index = row * sqrtNumZones + col;
	if (row < 0 || col < 0 || row>sqrtNumZones || col > sqrtNumZones || row * sqrtNumZones + col > zones.size()) {
		return nullptr;
	}
	return zones[index];
}

vector<Zone*> Environment::neighbours(Zone* zone)
{
	vector<Zone*> neighbours;
	neighbours.push_back(zone);
	Zone* neighbour = zoneAt(Vector2f(zone->xMid - zoneWidth, zone->yMid - zoneHeight));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(Vector2f(zone->xMid - zoneWidth, zone->yMid));
	if (neighbour != nullptr) {
		neighbours.push_back(neighbour);
	}

	neighbour = zoneAt(Vector2f(zone->xMid - zoneWidth, zone->yMid + zoneHeight));
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

	neighbour = zoneAt(Vector2f(zone->xMid + zoneWidth, zone->yMid - zoneHeight));
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
	return neighbours;
}

void Environment::update()
{
	for (int i = 0; i < numZones; i++)
	{
		toRemove.clear();
		Zone* zone = zones[i];
		for (auto &entityC : zone->entities) {
			Entity* entity = entityC.second;
			if (!zone->legalPosition(entity->position)) {
				toRemove.push_back(entity);
				Zone* newZone = zoneAt(entity->position);
				newZone->entities[entity->id] = entity;
				entity->zone = newZone;
			}
		}
		for (auto &entity : toRemove) {
			zone->entities.erase(entity->id);
		}
	}
}

void Environment::draw(Vector2i upperLeft, Vector2i lowerRight)
{
	bool drawLines = entities.size() < 1000;
	Entity* closest = NULL;
	if (rects == NULL) {
		rects = new VertexArray(sf::Quads, 4 * entities.size());
	}

	if (drawLines) {
		if (lines == NULL) {
			lines = new VertexArray(sf::Lines, entities.size()*entities.size() * 2);
		}
	}
	else
		if (lines == NULL) {
			lines = new VertexArray(sf::Lines, sqrtNumZones * 2);
		}

	lines->clear();
	if (numZones < 256)
		drawZones();
	int innerCount = 0;
	int idx = 0, lidx = 0;

	for (int i = 0; i < numZones; i++)
	{
		Zone* uZone = zones[i];
		for (auto &entityC : uZone->entities)
		{
			float distance = 90000000;
			Entity* entity = entityC.second;
			for (Zone* zone : uZone->neighbours) {
				float dist = -1;
				for (auto &neighbourC : zone->entities)
				{
					Entity* neighbour = neighbourC.second;
					if (entity->id != neighbour->id) {
						int dist = pow(entity->position.x - neighbour->position.x, 2) + pow(entity->position.y - neighbour->position.y, 2);
						if (drawLines) {
							lines->append(entity->position);
							(*lines)[lidx++].color = Color::Green;
							lines->append(neighbour->position);
							(*lines)[lidx++].color = Color::Blue;
						}
						if (distance > dist) {
							distance = dist;
							closest = neighbour;
						}
					}
				}
			}
			if (closest != nullptr) {
				Vector2f dir = Vector2f(entity->position.x - closest->position.x > 0 ? 1 : -1, entity->position.y - closest->position.y > 0 ? 1 : -1);
				Vector2f newPos = entity->position + dir;
				if (legalPosition(newPos)) {
					entity->position = newPos;
				}
			}
			(*rects)[idx].color = entity->color;
			(*rects)[idx++].position = Vector2f(entity->position.x - entity->size.x, entity->position.y - entity->size.y);
			(*rects)[idx].color = entity->color;
			(*rects)[idx++].position = Vector2f(entity->position.x + entity->size.x, entity->position.y - entity->size.y);
			(*rects)[idx].color = entity->color;
			(*rects)[idx++].position = Vector2f(entity->position.x + entity->size.x, entity->position.y + entity->size.y);
			(*rects)[idx].color = entity->color;
			(*rects)[idx++].position = Vector2f(entity->position.x - entity->size.x, entity->position.y + entity->size.y);
		}
	}
	window->draw(*lines);
	window->draw(*rects);
}

bool Environment::legalPosition(Vector2f position)
{
	return 0 < position.x && position.x < size.x && 0 < position.y && position.y < size.y;
}

void Environment::drawZones()
{
	int idx = 0, ldx = 0;
	Text countText;
	countText.setFont(AppConfig::getInstance().mainFont);
	countText.setCharacterSize(16);
	countText.setFillColor(Color::Green);
	countText.setStyle(Text::Bold);
	for (int i = 0; i < numZones; i++)
	{
		Zone* zone = zones[i];
		countText.setPosition(Vector2f(zone->xStart, zone->yStart));
		countText.setString(to_string(zone->id) + ": #" + to_string(zone->entities.size()));
		window->draw(countText);
	}
	for (int i = 0; i < sqrtNumZones; i++) {
		lines->append(Vector2f(0, i * zoneHeight));
		lines->append(Vector2f(size.x, i * zoneHeight));
		lines->append(Vector2f(i * zoneWidth, 0));
		lines->append(Vector2f(i * zoneWidth, size.y));
	}
}
