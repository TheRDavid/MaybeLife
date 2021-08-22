#pragma once
#include "Entity.h"

class FoodSource :
	public Entity
{
public:
	float m_radius, m_spawnRate;
	int m_lifeSpan;
	FoodSource(Environment* environment, sf::Vector2f position);
	void update() override;
};

