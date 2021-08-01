#include "DeveloperToolsInputManager.h"
#include "AppConfig.h"
bool DeveloperToolsInputManager::catchInput()
{
	if (KEY_DOWN(Keyboard::D)) {
		std::cout << "DEV CONSOLE ON" << std::endl;
		return true;
	}
	else if (KEY_DOWN(Keyboard::F)) {
		std::cout << "SHOW FPS" << std::endl;
		AppConfig::getInstance().showFPS = !AppConfig::getInstance().showFPS;
	}
	return false;
}
