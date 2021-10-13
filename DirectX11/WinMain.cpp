#include "DxException.h"
#include "GameApp.h"

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
	catch(DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), "HR Failed", MB_OK);
	}

	return -1;
}
