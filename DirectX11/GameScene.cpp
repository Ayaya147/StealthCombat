#include "GameScene.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "GameApp.h"
#include "Renderer.h"
#include "Actor.h"
#include "PlayerActor.h"
#include "Timer.h"
#include "MeshComponent.h"
#include "Mesh.h"

namespace dx = DirectX;

GameScene::GameScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	PlayerActor* player = new PlayerActor(this, "Assets\\Models\\test11.obj");
	player->SetPosition(dx::XMFLOAT3{ 4.0f,0.0f,5.0f });
	player = new PlayerActor(this, "Assets\\Models\\suzanne.obj");
	player->SetPosition(dx::XMFLOAT3{ 2.0f,0.0f,5.0f });
	player = new PlayerActor(this, "Assets\\Models\\test11.obj");
	player->SetPosition(dx::XMFLOAT3{ 0.0f,0.0f,5.0f });
	player = new PlayerActor(this, "Assets\\Models\\suzanne.obj");
	player->SetPosition(dx::XMFLOAT3{ -2.0f,0.0f,5.0f });
	player = new PlayerActor(this, "Assets\\Models\\test11.obj");
	player->SetPosition(dx::XMFLOAT3{ -4.0f,0.0f,5.0f });
}

GameScene::~GameScene()
{
}

void GameScene::ProcessInput()
{
	BaseScene::ProcessInput();
}

void GameScene::Update()
{
	BaseScene::Update();
}

void GameScene::GenerateOutput()
{
	BaseScene::GenerateOutput();

	//if (1)
	//{
	//	Parameter parameter;
	//	mSceneManager->ChangeScene(SceneType::EGame, parameter, true);
	//}
}
