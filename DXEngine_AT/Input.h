#pragma once

#include <windows.h>
#include <unordered_map>

class Input
{
public:
	enum KEY
	{
		KEY_0 = '0',
		KEY_1 = '1',
		KEY_2 = '2',
		KEY_3 = '3',
		KEY_4 = '4',
		KEY_5 = '5',
		KEY_6 = '6',
		KEY_7 = '7',
		KEY_8 = '8',
		KEY_9 = '9',
		A     = 'A',
		B     = 'B',
		C     = 'C',
		D     = 'D',
		E     = 'E',
		F     = 'F',
		G     = 'G',
		H     = 'H',
		I     = 'I',
		J     = 'J',
		K     = 'K',
		L     = 'L',
		M     = 'M',
		N     = 'N',
		O     = 'O',
		P     = 'P',
		Q     = 'Q',
		R     = 'R',
		S     = 'S',
		T     = 'T',
		U     = 'U',
		V     = 'V',
		W     = 'W',
		X     = 'X',
		Y     = 'Y',
		Z     = 'Z',
		SPACE = VK_SPACE,
		ESCAPE = VK_ESCAPE,
	};

	Input() = default;
	~Input() = default;

	void keyDown(WPARAM wParam);
	void keyUp(WPARAM wParam);

	bool isPressed(KEY _key);

private:
	std::unordered_map<WPARAM, bool> keyMap = 
	{
		{KEY_0, 0},
		{KEY_1, 0},
		{KEY_2, 0},
		{KEY_3, 0},
		{KEY_4, 0},
		{KEY_5, 0},
		{KEY_6, 0},
		{KEY_7, 0},
		{KEY_8, 0},
		{KEY_9, 0},
		{A,     0},
		{B,     0},
		{C,     0},
		{D,     0},
		{E,     0},
		{F,     0},
		{G,     0},
		{H,     0},
		{I,     0},
		{J,     0},
		{K,     0},
		{L,     0},
		{M,     0},
		{N,     0},
		{O,     0},
		{P,     0},
		{Q,     0},
		{R,     0},
		{S,     0},
		{T,     0},
		{U,     0},
		{V,     0},
		{W,     0},
		{X,     0},
		{Y,     0},
		{Z,     0},
		{SPACE, 0},
		{ESCAPE,0},
	};  
};

