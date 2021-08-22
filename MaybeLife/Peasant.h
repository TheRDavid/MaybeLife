#pragma once
#include "Worker.h"
class Peasant :
	public Worker
{
public:
	Peasant(Environment* environment, sf::Vector2f position);
};

