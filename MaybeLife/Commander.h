#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_set>

#include "Entity.h"
#include "Zone.h"
#include "Element.h"
#include "GUI.h"

class Environment;
class Commander
{
public:
	Environment* m_environment;
	sf::RenderWindow* m_window;
	sf::View* m_sceneView, *m_guiView;
	gui::GUI* gui;

	std::unordered_set<unsigned int> guiBlockers;

	float m_numGoodGuys = 0;
	float m_numBadGuys = 0;

	float goodToBadRatio();

	void setEntityCollision(bool enabled);
	void setGUIVisible(bool visible);
	void setZonesVisible(bool visible);

	std::shared_ptr<Entity> entityAt(sf::Vector2f position);
	Zone* zoneAt(sf::Vector2f position);

	void setSelectedZone(Zone* zone);
	void selectZoneAt(sf::Vector2f position);

	void setSelectedEntity(std::shared_ptr<Entity> entity);
	void selectEntityAt(sf::Vector2f position);
	void deSelectEntity();
	std::weak_ptr<Entity> getSelectedEntity();

	void setSelectedEntity_hover(std::shared_ptr<Entity> entity);
	void selectEntityAt_hover(sf::Vector2f position);

	void setSelectedZone_hover(Zone* zone);
	void selectZoneAt_hover(sf::Vector2f position);

	void addEntity(std::shared_ptr<Entity> entity);
	void deleteEntity(std::shared_ptr<Entity> entity);

	void playSimulation();
	void pauseSimulation();
	void stepSimulation();
	void stopSimulation();

	int getCurrentSimulationStep();

	void blockCursor(unsigned int guiID);
	void unblockCursor(unsigned int guiID);
	bool isGUIBlockingCursor();
	bool GUIHasFocus();
	void requestFocus(Element* element);
	void deRequestFocus(Element* element);

	void displayRecordedFrame(int frame);
	void displayLiveSimulation();

	static Commander& getInstance() {
		static Commander theInstance;
		return theInstance;
	}

};

