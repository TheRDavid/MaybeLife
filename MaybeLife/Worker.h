#pragma once
#include "Person.h"

#include <math.h>

#include "FoodItem.h"
class Worker :
	public Person
{
public:
	float m_carryStrength;
	float m_gatheredNutrition;

	Worker(Environment* environment, sf::Vector2f position, sf::Vector2f size, float health, float speed, bool good, float carryStrength);

	void pickUp(FoodItem* food);
};

