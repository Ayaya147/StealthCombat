#include "GameScene.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "GameApp.h"
#include "Renderer.h"
#include "Actor.h"

GameScene::~GameScene()
{
	
}

void GameScene::ProcessInput()
{
}

void GameScene::Update()
{
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
	mActors.emplace_back(std::shared_ptr<Actor>{actor});
}
