#include "ApplicationToolsInputManager.h"

bool ApplicationToolsInputManager::catchInput()
{
	if (KEY_DOWN(Keyboard::Q)) {
		std::cout << "QUIT" << std::endl;
		exit(0);
	}
	return false;
}
