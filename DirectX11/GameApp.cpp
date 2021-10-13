#include "GameApp.h"
#include "Window.h"

GameApp::GameApp()
{
	mWindow = std::make_unique<Window>(1280, 720);
}

GameApp::~GameApp()
{
}

int GameApp::Run()
{
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

		}
	}

	return static_cast<int>(msg.wParam);
}
