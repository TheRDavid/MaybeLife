#pragma once
#include <SFML/Graphics.hpp>
#include "Environment.h"
using namespace std;
class InputManager
{
public:
	InputManager(Environment* environment, RenderWindow* window, sf::View* sceneView, sf::View* uiView);
	Environment* environment;
	RenderWindow* window;
	sf::View* sceneView, *uiView;
	float currentZoom = 4;

	enum BaseCommand {
		set_behaviour, // random, spread
		set_gravity_center, // x, y
		// Switches: 0 -> false, 1 -> true
		show_lines,
		show_ui,
		show_zones,
		set_collide,

		invalid
	};

	void setBehaviour(string behaviour);
	void setGravityCenter(string x, string y);

	void handleEvents();
private:
	Vector2f startDragPos, endDragPos;
	bool dragging = false;
	void catchInput();
	void handleKeyboardCommands(sf::Event event);
	void execute(vector<string> tokens);
	Zone* zone;

	BaseCommand resolveCommand(std::string input) {
		if (input == "setBehaviour") return set_behaviour;
		if (input == "showLines") return show_lines;
		if (input == "showUI") return show_ui;
		if (input == "showZones") return show_zones;
		if (input == "setGravityCenter") return set_gravity_center;
		if (input == "setCollide") return set_collide;
		return invalid;
	}
};

