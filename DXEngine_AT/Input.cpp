#include "Input.h"

void Input::keyDown(WPARAM wParam)
{
	keyMap.at(wParam) = true;
}

void Input::keyUp(WPARAM wParam)
{
	keyMap.at(wParam) = false;
}

bool Input::isPressed(KEY _key)
{
	return keyMap.at(_key);
}
