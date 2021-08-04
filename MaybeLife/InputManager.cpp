#include "InputManager.h"
#include "InputMacros.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <thread>

InputManager::InputManager(Environment* environment)
{
	this->environment = environment;
	new thread(&InputManager::catchInput, this);
}

void InputManager::setBehaviour(string behaviour)
{
	Entity::Behaviour newBehaviour;
	if (behaviour == "gravitate") {
		newBehaviour = Entity::Behaviour::GRAVITATE;
	}
	else if (behaviour == "random") {
		newBehaviour = Entity::Behaviour::RANDOM;
	}
	else if (behaviour == "sleep") {
		newBehaviour = Entity::Behaviour::SLEEP;
	}
	else if (behaviour == "spread") {
		newBehaviour = Entity::Behaviour::SPREAD;
	}
	else if (behaviour == "fall") {
		newBehaviour = Entity::Behaviour::FALL;
	}
	else {
		cout << "ERROR: Invalid Behaviour" << endl;
		newBehaviour = Entity::Behaviour::RANDOM;
	}
	for (Entity* entity : *(environment->entities)) {
		entity->behaviour = newBehaviour;
	}
}

void InputManager::setMaxLines(string maxLines)
{
	string errorMsg = "ERROR: Invalid Number of Lines";
	try {
		int newMaxLines = stoi(maxLines);

		if (newMaxLines >= 0) {
			environment->setMaximumNumberOfLines(newMaxLines);
			return;
		}
	}
	catch (const std::invalid_argument&) {
		cout << errorMsg << endl;
	}
	cout << errorMsg << endl;
}

void InputManager::setGravityCenter(string x, string y)
{
	string errorMsg = "ERROR: Invalid Gravity Center";
	try {
		int centerX = stoi(x);
		int centerY = stoi(y);

		environment->gravityCenter = Vector2f(centerX, centerY);
		return;
	}
	catch (const std::invalid_argument&) {
		cout << errorMsg << endl;
	}
	cout << errorMsg << endl;
}

void InputManager::catchInput()
{
	while (true) {
		cout << ":";
		string cmd;
		getline(cin, cmd);
		istringstream iss(cmd);
		vector<string> tokens{ istream_iterator<string>{iss},
					  istream_iterator<string>{} };
		execute(tokens);
	}
}

void InputManager::execute(vector<string> tokens)
{
	if (tokens.size() == 0) return;
	string command = tokens[0];
	switch (resolveCommand(command)) {

	case set_max_lines:
		if (tokens.size() != 2)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		setMaxLines(tokens[1]); break;

	case set_behaviour:
		if (tokens.size() != 2)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		setBehaviour(tokens[1]); break;

	case set_collide:
		if (tokens.size() != 2)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		environment->entityCollision = tokens[1] == "true"; break;

	case show_zones:
		if (tokens.size() != 2)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		environment->showZones = tokens[1] == "true"; break;

	case show_ui:
		if (tokens.size() != 2)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		environment->showUI = tokens[1] == "true"; break;

	case show_lines:
		if (tokens.size() != 2)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		environment->showLines = tokens[1] == "true"; break;

	case set_gravity_center:
		if (tokens.size() != 3)
		{
			cout << "ERROR: Invalid Number of Arguments" << endl;
			break;
		}
		setGravityCenter(tokens[1], tokens[2]); break;

	default:
		cout << "ERROR: Invalid Command" << endl;
	}
}
