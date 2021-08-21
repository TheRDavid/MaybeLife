#pragma once

#include "json.hpp"
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <SFML/Graphics.hpp>

#include "Entity.h"

using json = nlohmann::json;
class SimConfig {
public:

	int getNumEntities() {
		return m_data["numEntities"];
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