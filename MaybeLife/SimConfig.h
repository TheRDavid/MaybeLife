#pragma once

#include "json.hpp"
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <SFML/Graphics.hpp>

#include "Entity.h"

using json = nlohmann::json;
class SimConfig {
public:

	int getNumGoodGuys() {
		return m_data["goodGuys"];
	}

	int getNumBadGuys() {
		return m_data["badGuys"];
	}

	int getNumPeasants() {
		return m_data["peasants"];
	}

	int getNumSlaves() {
		return m_data["slaves"];
	}

	int getNumFoodSources() {
		return m_data["numFoodSources"];
	}

	int getGoodGuysStartNutrition() {
		return m_data["goodGuysStartNutrition"];
	}

	int getBadGuysStartNutrition() {
		return m_data["badGuysStartNutrition"];
	}

	int getNumZones() {
		return m_data["numZones"];
	}

	int getNumThreads() {
		return m_data["numThreads"];
	}

	static SimConfig& getInstance() {
		static SimConfig m_theInstance;
		return m_theInstance;
	}
private:

	json m_data;

	SimConfig() {
		std::ifstream("config/sim.json") >> m_data;
		std::cout << "Sim Config: " << m_data << std::endl;
	}

};