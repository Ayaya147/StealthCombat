#include "GameScene.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "GameApp.h"
#include "Renderer.h"

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

	if (0)
	{
		Parameter parameter;
		mSceneManager->ChangeScene(SceneType::EGame, parameter, true);
	}
}
