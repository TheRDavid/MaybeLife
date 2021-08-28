#include "Utilities.h"

#include "Entity.h"
#include "Base.h"
#include "Worker.h"
#include "BadGuy.h"
#include "GoodGuy.h"
#include "Slave.h"
#include "Peasant.h"
#include "Person.h"
#include "Fighter.h"
#include "Wall.h"
#include "FoodItem.h"
#include "FoodSource.h"

std::shared_ptr<Entity> ut::convertToEntity(nlohmann::json data)
{
	std::shared_ptr<Entity> entity;
	std::string type = data["type"];

	if (type == "BadGuy")
	{
		std::shared_ptr<BadGuy> bentity = std::make_shared<BadGuy>();
		writeIntoBadGuy(bentity, data);
		entity = bentity;
	}
	else if (type == "Base")
	{
		std::shared_ptr<Base> bentity = std::make_shared<Base>();
		writeIntoBase(bentity, data);
		entity = bentity;
	}
	else if (type == "FoodItem")
	{
		std::shared_ptr<FoodItem> bentity = std::make_shared<FoodItem>();
		writeIntoFoodItem(bentity, data);
		entity = bentity;
	}
	else if (type == "FoodSource")
	{
		std::shared_ptr<FoodSource> bentity = std::make_shared<FoodSource>();
		writeIntoFoodSource(bentity, data);
		entity = bentity;
	}
	else if (type == "GoodGuy")
	{
		std::shared_ptr<GoodGuy> bentity = std::make_shared<GoodGuy>();
		writeIntoGoodGuy(bentity, data);
		entity = bentity;
	}
	else if (type == "Peasant")
	{
		std::shared_ptr<Peasant> bentity = std::make_shared<Peasant>();
		writeIntoPeasant(bentity, data);
		entity = bentity;
	}
	else if (type == "Slave")
	{
		std::shared_ptr<Slave> bentity = std::make_shared<Slave>();
		writeIntoSlave(bentity, data);
		entity = bentity;
	}
	else if (type == "Wall")
	{
		std::shared_ptr<Wall> bentity = std::make_shared<Wall>();
		writeIntoWall(bentity, data);
		entity = bentity;
	}

	if (type.compare("Entity") == 0)
	{
		std::cout << "skipping read entity of " << data["id"] << " for it has no goddamn type" << std::endl;
	}
	else {

		writeIntoEntity(entity, data);
	}

	return entity;
}

//TODO: Get zone via zone id, currently zones are ignored, as they are not required to draw the entities
void ut::writeIntoEntity(std::shared_ptr<Entity> entity, nlohmann::json data)
{
	entity->m_id = data["id"];
	entity->m_age = data["age"];
	entity->m_name = data["name"];
	entity->m_collide = data["collide"];
	entity->m_color = ut::to_color(data["color"]);
	entity->m_enabled = data["enabled"];
	entity->m_position = ut::to_vector2f(data["position"]);
	entity->m_size = ut::to_vector2f(data["size"]);
	entity->m_shape = data["shape"];
}

void ut::writeIntoBase(std::shared_ptr<Base> entity, nlohmann::json data)
{
	entity->m_good = data["good"];
	entity->m_nutrition = data["nutrition"];
	entity->m_workerSpawnRate = data["workerSpawnRate"];
	entity->m_fighterSpawnRate = data["fighterSpawnRate"];
}

void ut::writeIntoBadGuy(std::shared_ptr<BadGuy> entity, nlohmann::json data)
{
	writeIntoFighter(entity, data);
	// well... maybe later this will have use?
}

void ut::writeIntoFighter(std::shared_ptr<Fighter> entity, nlohmann::json data)
{
	writeIntoPerson(entity, data);
	entity->m_attack = data["attack"];
}

void ut::writeIntoFoodItem(std::shared_ptr<FoodItem> entity, nlohmann::json data)
{
	entity->m_nutrition = data["nutrition"];
}

void ut::writeIntoFoodSource(std::shared_ptr<FoodSource> entity, nlohmann::json data)
{
	entity->m_radius = data["radius"];
	entity->m_spawnRate = data["spawnRate"];
	entity->m_lifeSpan = data["lifeSpan"];
}

void ut::writeIntoGoodGuy(std::shared_ptr<GoodGuy> entity, nlohmann::json data)
{
	writeIntoFighter(entity, data);
	// well... maybe later this will have use?
}

void ut::writeIntoPeasant(std::shared_ptr<Peasant> entity, nlohmann::json data)
{
	writeIntoWorker(entity, data);
	// well... maybe later this will have use?
}

//TODO: missing base, as that is not needed for drawing and requires an actual reference
//TODO: also missing inView list :(
void ut::writeIntoPerson(std::shared_ptr<Person> entity, nlohmann::json data)
{
	writeIntoEntity(entity, data);
	entity->m_health = data["health"];
	entity->m_speed = data["speed"];
	entity->m_viewDistance = data["viewDistance"];
	entity->m_wanderStrength = data["wanderStrength"];
	entity->m_good = data["good"];
	entity->maxInMind = data["maxInMind"];
}

void ut::writeIntoSlave(std::shared_ptr<Slave> entity, nlohmann::json data)
{
	writeIntoWorker(entity, data);
	// well... maybe later this will have use?
}

void ut::writeIntoWall(std::shared_ptr<Wall> entity, nlohmann::json data)
{
	entity->m_health = data["health"];
}

void ut::writeIntoWorker(std::shared_ptr<Worker> entity, nlohmann::json data)
{
	writeIntoPerson(entity, data);
	entity->m_carryStrength = data["carryStrength"];
	entity->m_gatheredNutrition = data["gatheredNutrition"];
}