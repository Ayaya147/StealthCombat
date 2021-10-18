#include <Windows.h>
#include "GameApp.h"

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	GameApp{}.Run();
	return 0;
}
