#pragma once
#include "Entity.h"
class Wall :
	public Entity
{
public:
	float m_health = 250;

	Wall(Environment* environment, sf::Vector2f position);
	Wall() {} // for json conversion
	virtual void jsonify(nlohmann::json* data) override;
};

