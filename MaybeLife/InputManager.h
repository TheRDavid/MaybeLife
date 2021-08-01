#pragma once
#include <SFML/Graphics.hpp>
#include "DeveloperToolsInputManager.h"
#include "ApplicationToolsInputManager.h"
#include "SimulationToolsInputManager.h"
class InputManager
{
public:
	/*
	tab		-> dev tool shortcut
	control -> application shortcut
	shift	-> simulation shortcut
*/
	bool catchInput();
private:
	DeveloperToolsInputManager devInput;
	ApplicationToolsInputManager appInput;
	SimulationToolsInputManager simInput;
};

