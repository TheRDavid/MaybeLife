#pragma once
#include "Fighter.h"
class BadGuy :
	public Fighter
{
public:
	BadGuy(Environment* environment, sf::Vector2f position);
};

