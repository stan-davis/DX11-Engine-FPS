#include <Windows.h>
#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Game game(hInstance);
	HRESULT result = S_OK; //Error checking

	result = game.InitWindow();
	if (FAILED(result)) return result;

	result = game.InitDirectX();
	if (FAILED(result)) return result;

	return game.Run();
}