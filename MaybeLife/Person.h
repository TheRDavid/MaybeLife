#pragma once
#include "Entity.h"

#include <SFML/Graphics.hpp>
#include <mutex>
#include <unordered_set>

#include "Base.h"
class Person :
	public Entity
{
public:
	std::shared_ptr<Base> m_base;
	float m_health, m_speed, m_viewDistance, m_wanderStrength, lastTurn = 0;
	bool m_good;
	int maxInMind = 20;
	sf::Vector2f m_gazeDirection, m_wanderDirection;
	std::map<int, std::shared_ptr<Entity>> m_toAdd;
	std::unordered_set<int> m_toRemove;
	std::map<int, std::shared_ptr<Entity>> m_inViewDistance;
	Person(Environment* environment, sf::Vector2f position, sf::Vector2f size, float speed, bool good, float viewDistance, float wanderStrength, std::shared_ptr<Base> base);
	Person() {} // for json conversion

	void move(sf::Vector2f dir);
	void wander();
	void walkToBase();
	virtual void update() override;
	virtual void jsonify(nlohmann::json* data) override;
	std::mutex viewLock, toAddLock, toRemoveLock;
};

