#pragma once
#include "Entity.h"
class FoodItem :
	public Entity
{
public:
	float m_nutrition;
	FoodItem(Environment* environment, sf::Vector2f position);
	FoodItem() {} // for json conversion
	virtual void update() override;
	virtual void jsonify(nlohmann::json* data) override;
};

