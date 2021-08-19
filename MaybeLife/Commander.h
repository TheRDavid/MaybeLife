#pragma once
#include <string>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Zone.h"

class Environment;
class Commander
{
public:
	Commander(Environment* environment, sf::RenderWindow* window, sf::View* sceneView, sf::View* uiView);

	void setEntityCollision(bool enabled);
	void setBehaviour(std::string behaviour);
	void setUIVisible(bool visible);
	void setZonesVisible(bool visible);
	void setGravityCenter(std::string x, std::string y);

	void setSelectedZone(Zone* zone);
	void selectZoneAt(sf::Vector2f position);
	void setSelectedEntity(Entity* entity);
	void addEntity(sf::Vector2f position);

private:
	Environment* environment;
	sf::RenderWindow* window;
	sf::View* sceneView, *uiView;

	Zone* selectedZone;
	Entity* selectedEntity;
};

