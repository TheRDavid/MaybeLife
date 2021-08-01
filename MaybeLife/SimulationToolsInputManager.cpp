#include "SimulationToolsInputManager.h"

bool SimulationToolsInputManager::catchInput()
{
	
	if (KEY_DOWN(Keyboard::C)) {
		std::cout << "RESTART" << std::endl;
		return true;
	}
	return false;
}
