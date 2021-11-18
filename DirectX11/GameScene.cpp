#include "GameScene.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "GameApp.h"
#include "Renderer.h"
#include "Actor.h"
#include "PlayerActor.h"

GameScene::GameScene(SceneManager* sm, const Parameter& parameter)
	:
	AbstractScene(sm, parameter)
{
	PlayerActor* player = new PlayerActor(this);
}

GameScene::~GameScene()
{
}

void GameScene::ProcessInput()
{
}

void GameScene::Update()
{
	for (auto actor : mActors)
	{
		actor->Update();
	}
}

void GameScene::GenerateOutput()
{
	mSceneManager->GetApp()->GetRenderer()->Draw();

	//if (1)
	//{
	//	Parameter parameter;
	//	mSceneManager->ChangeScene(SceneType::EGame, parameter, true);
	//}
}

void GameScene::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void GameScene::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		mActors.erase(iter);
	}
}
