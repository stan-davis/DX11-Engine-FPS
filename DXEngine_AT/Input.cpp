#include "Input.h"

Input::Input()
{
	//Create keyState map - this could be done at compile time so this needs improving but it works well enough

	for (int i = 0x1B; i <= 0x5A; i++)
	{
		keyState.insert(std::pair<int, bool>(i, false));
	}
}

void Input::keyDown(WPARAM wParam)
{
	keyState[wParam] = true;
}

void Input::keyUp(WPARAM wParam)
{
	keyState[wParam] = false;
}

bool Input::isPressed(int key)
{
	return keyState[key];
}
