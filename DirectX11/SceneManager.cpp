#include "SceneManager.h"
#include "Parameter.h"
#include "GameScene.h"
#include "Renderer.h"
#include "Random.h"
#include "InputSystem.h"

SceneManager::SceneManager(Renderer* renderer, InputSystem* input)
	:
	mRenderer(renderer),
	mInputSystem(input)
{
	Parameter parameter;
	mSceneStack.emplace(new GameScene(this, parameter));

	mRenderer->SetScene(mSceneStack.top());

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
		break;
	case SceneType::EGame:
		mSceneStack.emplace(new GameScene(this, parameter));
		break;
	case SceneType::EResult:
		break;
	}

	mRenderer->SetScene(mSceneStack.top());
}
