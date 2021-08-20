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
		return data["numEntities"];
	}

	int getNumZones() {
		return data["numZones"];
	}

	int getNumThreads() {
		return data["numThreads"];
	}

	Entity::Behaviour getDefaultBehaviour() {
		return Entity::to_behaviour(data["defaultBehaviour"]);
	}

	static SimConfig& getInstance() {
		static SimConfig theInstance;
		return theInstance;
	}
private:

	json data;

	SimConfig() {
		std::ifstream("config/sim.json") >> data;
		std::cout << "Sim Config: " << data << std::endl;
	}

};