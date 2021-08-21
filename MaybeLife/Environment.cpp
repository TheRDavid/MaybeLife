#include "Environment.h"

#include <time.h>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <thread>
#include <iostream>

#include "AppConfig.h"
#include "Utilities.h"
#include "Commander.h"
#include "Grid.h"

Environment::Environment(sf::RenderWindow * renderWindow, sf::Vector2i size, int _numZones, int threads, sf::View* sceneView)
{
	this->sceneView = sceneView;
	centerShape.setRadius(gravityShapeRadius);
	centerShapeSize = sf::Vector2f(gravityShapeRadius / 2, gravityShapeRadius / 2);
	centerShape.setFillColor(sf::Color(255, 255, 255, 128));
	centerShape.setOutlineColor(sf::Color::Red);
	centerShape.setOutlineThickness(2);
	centerShape.setPosition(10, 10);
	gravityCenter = sf::Vector2f(size.x / 2, size.y / 2);
	numThreads = threads;
	steps = 0;
	int zoneCapacity = 2 * entities->size() / _numZones;

	entityGrid = new Grid(_numZones, zoneCapacity, size);

	showZones = entityGrid->numZones <= 1000;
	window = renderWindow;
	this->size = size;

	zoneLines = new sf::VertexArray(sf::Lines, (entityGrid->cols + entityGrid->rows) * 2);
}

void Environment::start(std::vector<Entity*>* entities) {
	this->entities = entities;


	for (Entity* entity : (*entities)) {
		entity->environment = this;
		Zone* zone = entityGrid->zoneAt(entity->position);
		zone->addEntityImmediatly(entity);
	}

	for (int i = 0; i < numThreads; i++) {
		int firstZone = i == 0 ? 0 : i * entityGrid->numZones / numThreads;
		int lastZone = i == 0 ? entityGrid->numZones / numThreads : (i + 1)*entityGrid->numZones / numThreads;
		std::cout << "New Thread: " << i << " zones " << firstZone << " to " << lastZone << std::endl;
		ranges.push_back(sf::Vector2i(firstZone, lastZone));
	}
	new std::thread(&Environment::updateEntities, this);
}

bool Environment::legalPosition(sf::Vector2f position)
{
	return 0 <= position.x && position.x <= size.x && 0 <= position.y && position.y <= size.y;
}

void Environment::adjustRenderingRect()
{
	renderRectPosition = sceneView->getCenter() - sf::Vector2f(sceneView->getSize().x / 2, sceneView->getSize().y / 2);
	renderRectSize = sf::Vector2f(sceneView->getSize().x, sceneView->getSize().y);
}

void Environment::updateZoneRange(int firstZone, int lastZone)
{
	for (int i = firstZone; i < lastZone; i++)
	{
		Zone* uZone = entityGrid->zones[i];
		uZone->update();
		for (Entity* entity : uZone->entities)
		{
			entity->update();
		}
	}
}

void Environment::updateEntities()
{
	while (true) {
		int rangeIndex = 0;
		for (auto range : ranges)
		{
			switch (rangeIndex++)
			{
			case 0: zoneProcessor_0 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 1: zoneProcessor_1 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 2: zoneProcessor_2 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 3: zoneProcessor_3 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 4: zoneProcessor_4 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 5: zoneProcessor_5 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 6: zoneProcessor_6 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 7: zoneProcessor_7 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 8: zoneProcessor_8 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 9: zoneProcessor_9 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 10: zoneProcessor_10 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 11: zoneProcessor_11 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 12: zoneProcessor_12 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 13: zoneProcessor_13 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 14: zoneProcessor_14 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			case 15: zoneProcessor_15 = std::async(&Environment::updateZoneRange, this, range.x, range.y);
				break;
			}
		}
		for (int i = 0; i< rangeIndex;i++)
		{
			switch (i)
			{
			case 0: zoneProcessor_0.wait();
				break;
			case 1: zoneProcessor_1.wait();
				break;
			case 2: zoneProcessor_2.wait();
				break;
			case 3: zoneProcessor_3.wait();
				break;
			case 4: zoneProcessor_4.wait();
				break;
			case 5: zoneProcessor_5.wait();
				break;
			case 6: zoneProcessor_6.wait();
				break;
			case 7: zoneProcessor_7.wait();
				break;
			case 8: zoneProcessor_8.wait();
				break;
			case 9: zoneProcessor_9.wait();
				break;
			case 10: zoneProcessor_10.wait();
				break;
			case 11: zoneProcessor_11.wait();
				break;
			case 12: zoneProcessor_12.wait();
				break;
			case 13: zoneProcessor_13.wait();
				break;
			case 14: zoneProcessor_14.wait();
				break;
			case 15: zoneProcessor_15.wait();
				break;
			}
		}
		steps++;
	}
}

void Environment::draw()
{
	window->setView(*sceneView);
	if (rects == NULL) {
		rects = new sf::VertexArray(sf::Quads, 4 * entities->size());
	}

	if (showZones)
		drawZones();

	rects->clear();
	int lineIndex = 0;
	int idx = 0;
	for (Entity* entity : *entities)
	{
		if (inRenderRect(entity)) {
			sf::Vector2f ePos = entity->position;
			sf::Vector2f eSize = entity->size;
			sf::Color col = entity->color;
			rects->append(sf::Vector2f(ePos.x - eSize.x, ePos.y - eSize.y));
			(*rects)[idx++].color = col;
			rects->append(sf::Vector2f(ePos.x + eSize.x, ePos.y - eSize.y));
			(*rects)[idx++].color = col;
			rects->append(sf::Vector2f(ePos.x + eSize.x, ePos.y + eSize.y));
			(*rects)[idx++].color = col;
			rects->append(sf::Vector2f(ePos.x - eSize.x, ePos.y + eSize.y));
			(*rects)[idx++].color = col;
		}
	}

	window->draw(*rects);
	centerShape.setPosition(sf::Vector2f(gravityCenter.x - gravityShapeRadius, gravityCenter.y - gravityShapeRadius));
	window->draw(centerShape);
}

std::string Environment::stepsToString()
{
	std::string str = "Steps: ";
	for (int i = 0; i < std::max(1, numThreads); i++) {
		str += " " + std::to_string(steps);
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
		for (Zone* neighbour : selectedZone->neighbours) {
			rectangle.setSize(sf::Vector2f(neighbour->xEnd - neighbour->xStart, neighbour->yEnd - neighbour->yStart));
			rectangle.setOutlineColor(sf::Color::Blue);
			rectangle.setOutlineThickness(1);
			rectangle.setFillColor(sf::Color(70, 70, 190, 50));
			rectangle.setPosition(neighbour->xStart, neighbour->yStart);
			window->draw(rectangle);
		}
	}

	int ldx = 0;
	sf::Color lC = sf::Color(150, 150, 150, 255);
	for (int i = 0; i < entityGrid->rows; i++) {
		(*zoneLines)[ldx].position = sf::Vector2f(0, i * entityGrid->zoneHeight);
		(*zoneLines)[ldx++].color = lC;
		(*zoneLines)[ldx].position = sf::Vector2f(size.x, i * entityGrid->zoneHeight);
		(*zoneLines)[ldx++].color = lC;
	}
	for (int i = 0; i < entityGrid->cols; i++) {
		(*zoneLines)[ldx].position = sf::Vector2f(i * entityGrid->zoneWidth, 0);
		(*zoneLines)[ldx++].color = lC;
		(*zoneLines)[ldx].position = sf::Vector2f(i * entityGrid->zoneWidth, size.y);
		(*zoneLines)[ldx++].color = lC;
	}
	window->draw(*zoneLines);
}

bool Environment::inRenderRect(Entity * entity)
{
	//std::cout << entity->to_bounds_string() + " in " << ut::to_string(renderRectPosition) << " x " << ut::to_string(renderRectSize) << std::endl;
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

