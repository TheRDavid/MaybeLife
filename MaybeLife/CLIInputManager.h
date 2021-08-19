#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>

#include "Environment.h"
#include "Zone.h"
#include "Commander.h"
enum BaseCommand {
	set_behaviour, // random, spread
	set_gravity_center, // x, y
	// Switches: 0 -> false, 1 -> true
	show_ui,
	show_zones,
	set_collide,

	invalid
};

class CLIInputManager
{
public:
	CLIInputManager(Commander* commander);

private:
	Commander* commander;

	void catchInput();
	BaseCommand resolveCommand(std::string input);
	void evaluate(std::vector<std::string> tokens);
};
