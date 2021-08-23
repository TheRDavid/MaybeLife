#pragma once
#include "Entity.h"
class Base :
	public Entity
{
public:
	bool m_good;
	float m_nutrition, m_workerSpawnRate, m_fighterSpawnRate;

	Base(Environment* environment, sf::Vector2f position, bool good, float startNutrition);
	void update() override;
};

