#pragma once
#include "Entity.h"
class Base :
	public Entity
{
public:
	bool m_good;
	float m_nutrition;

	Base(Environment* environment, sf::Vector2f position, bool good, float startNutrition);
};

