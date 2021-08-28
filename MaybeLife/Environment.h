#pragma once

#include <vector>
#include <mutex>
#include <future>
#include <fstream>
#include <unordered_set>

#include "json.hpp"

#include "Zone.h"
#include "Entity.h"
class Base;
class Grid;
class Environment
{

public:
	Grid* m_entityGrid;
	std::vector<std::shared_ptr<Entity>>* m_entities; 
	std::vector<std::shared_ptr<Entity>>* m_loadedEntities;
	std::vector<std::shared_ptr<Entity>>* m_toAdd;
	std::vector<std::shared_ptr<Entity>>* m_toRemove;
	sf::RenderWindow* m_window;

	bool m_liveView = true;
	int m_displayFrame = -1;

	std::string m_sessionDir;
	std::shared_ptr<Base> m_goodBase = nullptr, m_badBase = nullptr;

	int m_numThreads;

	bool tmp_print = false;
	bool m_initiateShutdown = false;
	bool m_showZones = false;
	bool m_paused = false, m_pauseAfterNextStep = false;
	bool m_showGUI = true;
	bool m_entityCollision = true;
	bool m_recording;

	sf::RectangleShape m_processedZoneRect;

	sf::Vector2f left = sf::Vector2f(-2, 0);
	sf::Vector2f upLeft = sf::Vector2f(-2, -2);
	sf::Vector2f up = sf::Vector2f(0, -2);
	sf::Vector2f upRight = sf::Vector2f(2, -2);
	sf::Vector2f right = sf::Vector2f(2, 0);
	sf::Vector2f downRight = sf::Vector2f(2, 2);
	sf::Vector2f down = sf::Vector2f(0, 2);
	sf::Vector2f downLeft = sf::Vector2f(-2, 2);
	sf::Vector2f gridDirections[8] = { left, upLeft, up, upRight, right, downRight, down, downLeft };

	std::mutex m_insertLock, m_shutdownLock;
	std::thread* m_updateThread;

	sf::Vector2i m_size;
	sf::Vector2f m_renderRectPosition;
	sf::Vector2f m_renderRectSize;

	Zone* m_selectedZone = nullptr;
	std::weak_ptr<Entity> m_selectedEntity;
	Zone* m_hoveredZone = nullptr;
	std::weak_ptr<Entity> m_hoveredEntity;
	std::vector<std::weak_ptr<Entity>> m_selectedEntities;

	Environment(sf::RenderWindow* renderWindow, sf::Vector2i size, int numZones, int threads, sf::View* sceneView, sf::View* guiView);
	void updateEntities();
	void updateZoneRange(int firstZone, int lastZone);
	void draw();
	int steps;
	void start(std::vector<std::shared_ptr<Entity>>* entities);
	std::string stepsToString();
	bool legalPosition(sf::Vector2f position);
	sf::Vector2f legalizePosition(sf::Vector2f position);
	void adjustRenderingRect();


private:
	std::vector<std::shared_ptr<Entity>>* m_displayEntities;
	int m_lastDisplayFrame = -1;
	std::unordered_set<int> recordedFrames;
	std::vector<nlohmann::json*>entityJs;

	sf::View* m_sceneView, *m_guiView;
	std::vector<sf::Vector2i> m_zoneProcessingRanges;
	sf::VertexArray* m_rects = NULL;
	sf::VertexArray* m_triangles = NULL;

	sf::VertexArray* m_zoneLines = NULL;
	sf::VertexArray* m_viewLines = NULL;

	int m_lastRecordedStep = 0;

	void drawZones();
	bool inRenderRect(std::shared_ptr<Entity> entity);
	bool inRenderRect(Zone* zone);

	std::vector<std::shared_ptr<Entity>>* loadFrame();

	std::future<void> zoneProcessor_0,
		zoneProcessor_1,
		zoneProcessor_2,
		zoneProcessor_3,
		zoneProcessor_4,
		zoneProcessor_5,
		zoneProcessor_6,
		zoneProcessor_7,
		zoneProcessor_8,
		zoneProcessor_9,
		zoneProcessor_10,
		zoneProcessor_11,
		zoneProcessor_12,
		zoneProcessor_13,
		zoneProcessor_14,
		zoneProcessor_15,
		zoneProcessor_16;
};


