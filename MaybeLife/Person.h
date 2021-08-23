#pragma once
#include "Entity.h"

#include <SFML/Graphics.hpp>
#include <mutex>

#include "Base.h"
class Person :
	public Entity
{
public:
	std::shared_ptr<Base> m_base;
	float m_health, m_speed, m_viewDistance, m_fov, m_wanderStrength, lastTurn = 0;
	bool m_good;
	sf::Vector2f m_gazeDirection, m_wanderDirection;
	std::map<int, std::weak_ptr<Entity>> m_inViewDistance;
	Person(Environment* environment, sf::Vector2f position, sf::Vector2f size, float speed, bool good, float viewDistance, float wanderStrength, std::shared_ptr<Base> base);

	void move(sf::Vector2f dir);
	void wander();
	void walkToBase();
	virtual void update() override;
	std::mutex viewLock;
};

