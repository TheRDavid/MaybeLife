#pragma once
#include "Worker.h"
class Slave :
	public Worker
{
public:
	Slave(Environment* environment, sf::Vector2f position, std::shared_ptr<Base> base);
	Slave() {} // for json conversion
	virtual void jsonify(nlohmann::json* data) override;
};

