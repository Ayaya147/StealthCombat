#include "GameApp.h"
#include "Window.h"
#include "Renderer.h"
#include "SceneManager.h"

GameApp::GameApp()
	:
	mWindow(std::make_unique<Window>(1280, 720)),
	mRenderer(std::make_unique<Renderer>(mWindow->GetHandle(), mWindow->GetClientWidth(), mWindow->GetClientHeight())),
	mSceneManager(std::make_unique<SceneManager>(this))
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
			mSceneManager->RunLoop();
		}
	}

	return static_cast<int>(msg.wParam);
}
