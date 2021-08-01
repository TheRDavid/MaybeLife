#include "Zone.h"
#include "Entity.h"
Zone::Zone() : Zone::Zone(-1,0, 0, 1, 1)
{
}

Zone::Zone(unsigned long long int id, float xStart, float xEnd, float yStart, float yEnd)
{
	this-> id = id;
	this->xStart = xStart;
	this->xEnd = xEnd;
	this->yStart = yStart;
	this->yEnd = yEnd;
	xMid = xStart + (xEnd - xStart) / 2;
	yMid = yStart + (yEnd - yStart) / 2;
	entities.reserve(100);
}

bool Zone::legalPosition(Vector2f position)
{
	return xStart <= position.x && position.x <= xEnd && yStart <= position.y && position.y <= yEnd;
}

std::string Zone::toString()
{
	return "Zone [" + std::to_string(xStart) + ", " + std::to_string(yStart) + ", " + std::to_string(xEnd) + ", " + std::to_string(yEnd) + "] #" + std::to_string(entities.size());
}
