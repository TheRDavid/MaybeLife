#pragma once
#include "Fighter.h"
class BadGuy :
	public Fighter
{
public:
	BadGuy(Environment* environment, sf::Vector2f position, std::shared_ptr<Base> base);
	BadGuy() {} // for json conversion
	virtual void jsonify(nlohmann::json* data) override;
};

