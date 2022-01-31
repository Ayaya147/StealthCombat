#include "SceneManager.h"
#include "Parameter.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "Renderer.h"
#include "Random.h"
#include "InputSystem.h"
#include "GameApp.h"
#include "Window.h"

SceneManager::SceneManager(GameApp* gameApp)
	:
	mApp(gameApp)
{
	Parameter parameter;
	mSceneStack.emplace(new GameScene(this, parameter));

	GetRenderer()->SetScene(mSceneStack.top());

	Random::Initialize();
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
	case SceneType::EGame:
		mSceneStack.emplace(new GameScene(this, parameter));
		break;
	case SceneType::EResult:
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

Window* SceneManager::GetWindow()
{
	return mApp->GetWindow();
}
