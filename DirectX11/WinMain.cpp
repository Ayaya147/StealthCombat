#include <Windows.h>
#include "GameApp.h"
#include "DxException.h"

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	try
	{
		return GameApp{}.Run();
	}
	catch (const DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), "HR Failed", MB_OK);
		return 0;
	}
}
