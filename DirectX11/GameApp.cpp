#include "GameApp.h"
#include "ImguiManager.h"
#include "InputSystem.h"
#include "AudioSystem.h"
#include "Window.h"
#include "Renderer.h"
#include "SceneManager.h"

ImguiManager* GameApp::mImgui = nullptr;
InputSystem* GameApp::mInputSystem = nullptr;
AudioSystem* GameApp::mAudioSystem = nullptr;
Window* GameApp::mWindow = nullptr;
Renderer* GameApp::mRenderer = nullptr;
SceneManager* GameApp::mSceneManager = nullptr;

GameApp::GameApp()
{
	mImgui = new ImguiManager();
	mInputSystem = new InputSystem();
	mAudioSystem = new AudioSystem();
	mWindow = new Window(1280, 720, mInputSystem);
	mRenderer = new Renderer(mWindow->GetHandle(), mWindow->GetClientWidth(), mWindow->GetClientHeight());
	mSceneManager = new SceneManager(this);
}

GameApp::~GameApp()
{
	delete mSceneManager;
	delete mRenderer;
	delete mWindow;
	delete mInputSystem;
	delete mAudioSystem;
	delete mImgui;
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
