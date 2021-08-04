#pragma once
#include <SFML/Graphics.hpp>
#include "Environment.h"
using namespace std;
class InputManager
{
public:
	InputManager(Environment* environment);
	Environment* environment;

	enum BaseCommand {
		set_behaviour, // random, spread
		set_max_lines, // maxNumberLines shown
		set_gravity_center, // x, y
		// Switches: 0 -> false, 1 -> true
		show_lines,
		show_ui,
		show_zones,
		set_collide,

		invalid
	};

	void setBehaviour(string behaviour);
	void setMaxLines(string maxLines);
	void setGravityCenter(string x, string y);
private:
	void catchInput();
	void execute(vector<string> tokens);

	BaseCommand resolveCommand(std::string input) {
		if (input == "setBehaviour") return set_behaviour;
		if (input == "showLines") return show_lines;
		if (input == "showUI") return show_ui;
		if (input == "showZones") return show_zones;
		if (input == "setMaxLines") return set_max_lines;
		if (input == "setGravityCenter") return set_gravity_center;
		if (input == "setCollide") return set_collide;
		return invalid;
	}
};

