#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>

#include "Environment.h"
#include "Zone.h"
#include "Commander.h"
enum BaseCommand {
	show_ui,
	show_zones,
	set_collide,

	invalid
};

class CLIInputManager
{
public:
	CLIInputManager();

private:

	void catchInput();
	BaseCommand resolveCommand(std::string input);
	void evaluate(std::vector<std::string> tokens);
};
