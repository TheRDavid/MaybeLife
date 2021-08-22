#pragma once
#include "Worker.h"
class Slave :
	public Worker
{
public:
	Slave(Environment* environment, sf::Vector2f position);
};

