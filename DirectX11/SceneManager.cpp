#include "SceneManager.h"
#include "Parameter.h"
#include "TitleScene.h"
#include "DemoScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "Renderer.h"
#include "Random.h"
#include "InputSystem.h"
#include "AudioSystem.h"
#include "GameApp.h"
#include "Window.h"

SceneManager::SceneManager(GameApp* gameApp)
	:
	mApp(gameApp)
{
	Random::Initialize();
	Parameter parameter;
	mSceneStack.emplace(new TitleScene(this, parameter));

	GetRenderer()->SetScene(mSceneStack.top());
}

SceneManager::~SceneManager()
{
	while (!mSceneStack.empty())
	{
		delete mSceneStack.top();
		mSceneStack.pop();
	}
}

void SceneManager::RunLoop()
{
	mSceneStack.top()->ProcessInput();
	mSceneStack.top()->Update();
	mSceneStack.top()->GenerateOutput();
}

void SceneManager::ChangeScene(SceneType scene, const Parameter& parameter, bool stackClear)
{
	if (stackClear)
	{
		while (!mSceneStack.empty())
		{
			delete mSceneStack.top();
			mSceneStack.pop();
		}
	}

	switch (scene)
	{
	case SceneType::ETitle:
		mSceneStack.emplace(new TitleScene(this, parameter));
		break;
	case SceneType::EDemo:
		mSceneStack.emplace(new DemoScene(this, parameter));
		break;
	case SceneType::EGame:
		mSceneStack.emplace(new GameScene(this, parameter));
		break;
	case SceneType::EResult:
		mSceneStack.emplace(new ResultScene(this, parameter));
		break;
	}

	GetRenderer()->SetScene(mSceneStack.top());
}

Renderer* SceneManager::GetRenderer()
{
	return mApp->GetRenderer();
}

InputSystem* SceneManager::GetInputSystem()
{
	return mApp->GetInputSystem();
}

AudioSystem* SceneManager::GetAudioSystem()
{
	return mApp->GetAudioSystem();
}

Window* SceneManager::GetWindow()
{
	return mApp->GetWindow();
}
