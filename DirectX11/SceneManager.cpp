#include "SceneManager.h"
#include "Parameter.h"
#include "GameScene.h"

SceneManager::SceneManager(GameApp* app)
	:
	mApp(app)
{
	Parameter parameter;
	mSceneStack.emplace(new GameScene(this, parameter));
}

SceneManager::~SceneManager()
{
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
}
