#pragma once

#include <windows.h>
#include "Keys.h"
#include <unordered_map>

class Input
{
public:
	Input();
	~Input() = default;

	void keyDown(WPARAM wParam);
	void keyUp(WPARAM wParam);

	bool isPressed(int key);

private:
	std::unordered_map<int, bool> keyState;
};