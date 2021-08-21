#include "Panel.h"

#include <iostream>

#include "Utilities.h"

using namespace gui;

Panel::Panel(sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, sf::Color backgroundColor, sf::Color edgeColor, int edgeThickness, bool draggable)
	: Element(window, position, size, draggable)
{
	this->backgroundColor = backgroundColor;
	this->edgeColor = edgeColor;
	this->edgeThickness = edgeThickness;
}

void Panel::drawSelf(sf::Vector2f relativePosition)
{
	quads.clear();
	triangleStrips.clear();

	sf::Vector2f thicknessVec = sf::Vector2f(edgeThickness, edgeThickness);
	sf::Vector2f upperLeft = position + relativePosition;
	sf::Vector2f upperRight = sf::Vector2f(upperLeft.x + size.x, upperLeft.y);
	sf::Vector2f lowerRight = upperLeft + size;
	sf::Vector2f lowerLeft = sf::Vector2f(upperLeft.x, upperLeft.y + size.y);

	triangleStrips.append(sf::Vector2f(upperLeft.x - edgeThickness, upperLeft.y-edgeThickness));
	triangleStrips.append(sf::Vector2f(upperLeft.x, upperLeft.y));
	triangleStrips.append(sf::Vector2f(upperRight.x, upperRight.y - edgeThickness));
	triangleStrips.append(sf::Vector2f(upperRight.x - edgeThickness , upperRight.y));
	triangleStrips.append(sf::Vector2f(lowerRight.x, lowerRight.y));
	triangleStrips.append(sf::Vector2f(lowerRight.x - edgeThickness, lowerRight.y-edgeThickness));
	triangleStrips.append(lowerLeft);
	triangleStrips.append(lowerLeft- thicknessVec);
	triangleStrips.append(sf::Vector2f(upperLeft.x, upperLeft.y));
	triangleStrips.append(sf::Vector2f(upperLeft.x - edgeThickness, upperLeft.y - edgeThickness));

	for (int v = 0; v < triangleStrips.getVertexCount(); v++) {
		triangleStrips[v].color = edgeColor;
	}

	quads.append(upperLeft);
	quads.append(upperRight);
	quads.append(lowerRight);
	quads.append(lowerLeft);

	for (int v = 0; v < quads.getVertexCount(); v++) {
		quads[v].color = backgroundColor;
	}

	window->draw(quads);
	window->draw(triangleStrips);
}
