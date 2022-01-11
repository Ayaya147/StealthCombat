#include "GameApp.h"
#include "InputSystem.h"
#include "Window.h"
#include "Renderer.h"
#include "SceneManager.h"

GameApp::GameApp()
	:
	mInputSystem(new InputSystem()),
	mWindow(new Window(1280, 720)),
	mRenderer(new Renderer(mWindow->GetHandle(), mWindow->GetClientWidth(), mWindow->GetClientHeight())),
	mSceneManager(new SceneManager(mRenderer))
{
}

GameApp::~GameApp()
{
	delete mSceneManager;
	delete mRenderer;
	delete mWindow;
	delete mInputSystem;
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
