#pragma once
#include "Fighter.h"
class GoodGuy :
	public Fighter
{
public:
	GoodGuy(Environment* environment, sf::Vector2f position, std::shared_ptr<Base> base);
	GoodGuy() {} // for json conversion
	virtual void jsonify(nlohmann::json* data) override;
};



