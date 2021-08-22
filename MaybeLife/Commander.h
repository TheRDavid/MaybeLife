#pragma once
#include <string>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Zone.h"

class Environment;
class Commander
{
public:
	Environment* m_environment;
	sf::RenderWindow* m_window;
	sf::View* m_sceneView, *m_guiView;

	void setEntityCollision(bool enabled);
	void setGUIVisible(bool visible);
	void setZonesVisible(bool visible);

	void setSelectedZone(Zone* zone);
	void selectZoneAt(sf::Vector2f position);
	void setSelectedEntity(Entity* entity);
	void addEntity(Entity* entity);
	void deleteEntity(Entity* entity);
	static Commander& getInstance() {
		static Commander theInstance;
		return theInstance;
	}

private:

	Zone* m_selectedZone;
	Entity* m_selectedEntity;
};

