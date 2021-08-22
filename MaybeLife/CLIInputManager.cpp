#include "CLIInputManager.h"

CLIInputManager::CLIInputManager()
{
	new std::thread(&CLIInputManager::catchInput, this);
}

void CLIInputManager::catchInput()
{
	while (true) {
		std::cout << ":";
		std::string cmd;
		std::getline(std::cin, cmd);
		std::istringstream iss(cmd);
		std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
					  std::istream_iterator<std::string>{} };
		evaluate(tokens);
	}
}

BaseCommand CLIInputManager::resolveCommand(std::string input) {
	if (input == "showUI") return show_ui;
	if (input == "showZones") return show_zones;
	if (input == "setCollide") return set_collide;
	return invalid;
}

void CLIInputManager::evaluate(std::vector<std::string> tokens)
{
	if (tokens.size() == 0) return;
	std::string command = tokens[0];
	switch (resolveCommand(command)) {

	case set_collide:
		if (tokens.size() != 2)
		{
			std::cout << "ERROR: Invalid Number of Arguments" << std::endl;
			break;
		}
		Commander::getInstance().setEntityCollision(tokens[1] == "true"); break;

	case show_zones:
		if (tokens.size() != 2)
		{
			std::cout << "ERROR: Invalid Number of Arguments" << std::endl;
			break;
		}
		Commander::getInstance().setZonesVisible(tokens[1] == "true"); break;

	case show_ui:
		if (tokens.size() != 2)
		{
			std::cout << "ERROR: Invalid Number of Arguments" << std::endl;
			break;
		}
		Commander::getInstance().setGUIVisible(tokens[1] == "true"); break;

	default:
		std::cout << "ERROR: Invalid Command" << std::endl;
	}
}