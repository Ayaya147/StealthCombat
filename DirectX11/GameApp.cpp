#include "GameApp.h"
#include "Window.h"
#include "Renderer.h"

GameApp::GameApp()
	:
	mWindow(std::make_unique<Window>(1280, 720)),
	mRenderer(std::make_unique<Renderer>(mWindow->GetHandle(), mWindow->GetClientWidth(), mWindow->GetClientHeight()))
{
}

GameApp::~GameApp()
{
}

int GameApp::Run()
{
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			mRenderer->Draw();
		}
	}

	return static_cast<int>(msg.wParam);
}
