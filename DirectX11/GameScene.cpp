#include "GameScene.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "GameApp.h"
#include "Renderer.h"
#include "Actor.h"
#include "PlayerActor.h"
#include "Timer.h"

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
	AbstractScene::ProcessInput();
}

void GameScene::Update()
{
	AbstractScene::Update();
}

void GameScene::GenerateOutput()
{
	AbstractScene::GenerateOutput();

	//if (1)
	//{
	//	Parameter parameter;
	//	mSceneManager->ChangeScene(SceneType::EGame, parameter, true);
	//}
}
