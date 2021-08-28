#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_set>

#include "Entity.h"
#include "Zone.h"
#include "Element.h"
#include "GUI.h"

class MouseInputManager;
class Environment;
class Commander
{
public:
	Environment* m_environment;
	MouseInputManager* m_mouseInputManager;
	sf::RenderWindow* m_window;
	sf::View* m_sceneView, *m_guiView;
	gui::GUI* gui;

	std::unordered_set<unsigned int> guiBlockers;

	int m_numGoodGuys = 0;
	int m_numBadGuys = 0;

	void registerMouseInputManager(MouseInputManager* mim);

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
	bool displayIsLiveSimulation();

	bool isRecording();
	void record();
	void stopRecording();

	sf::Vector2i getMousePixelPosition();

	sf::Vector2f getMouseWorldPosition();

	bool currentlyMakingArealSelection();
	bool activeMouseSelectionArea();
	ut::rectf mouseSelectionArea();
	void cancelArealSelection();
	std::vector<std::weak_ptr<Entity>> getSelectedEntities();

	sf::Vector2f convertWorldToGUICoordinates(sf::Vector2f worldCoords);

	static Commander& getInstance() {
		static Commander theInstance;
		return theInstance;
	}

};

