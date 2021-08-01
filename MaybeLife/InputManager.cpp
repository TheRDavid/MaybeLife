#include "InputManager.h"
#include "InputMacros.h"

bool InputManager::catchInput()
{
	if (TAB_DOWN) {
		return devInput.catchInput();
	}
	else if (CONTROL_DOWN) {
		return appInput.catchInput();
	}
	else if (SHIFT_DOWN) {
		return simInput.catchInput();
	}
	return false;
}
