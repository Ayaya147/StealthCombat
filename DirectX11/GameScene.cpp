#include "GameScene.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "Renderer.h"
#include "PlayerActor.h"
#include "CloudActor.h"
#include "PlaneActor.h"
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "Mesh.h"
#include "TransformCBuffer.h"
#include "Light.h"

namespace dx = DirectX;

GameScene::GameScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	Renderer* renderer = GetRenderer();

	renderer->SetAmbientLight(dx::XMFLOAT3{ 0.2f, 0.2f, 0.2f });
	DirectionalLightConstant dir = {};
	dir.mDirection = dx::XMFLOAT3{ 0.0f, -1.0f, -1.0f };
	dir.mDiffuseColor = dx::XMFLOAT3{ 0.8f, 0.9f, 1.0f };
	dir.mSpecColor = dx::XMFLOAT3{ 0.8f, 0.8f, 0.8f };
	renderer->SetDirectionalLight(dir);

	PlaneActor* plane = new PlaneActor(this);

	PlayerActor* player = new PlayerActor(this);
	CameraComponent* cc = new CameraComponent(player);
	player->SetPosition(dx::XMFLOAT3{ 0.0f,100.0f,0.0f });
	player->SetScale(0.1f);

	CloudActor* cloud = new CloudActor(this);
	cloud->SetPosition(dx::XMFLOAT3{ -5.0f,100.0f,0.0f });
	cloud->SetScale(dx::XMFLOAT3{ 4.0f,4.0f,4.0f });
	cloud = new CloudActor(this);
	cloud->SetPosition(dx::XMFLOAT3{ 5.0f,100.0f,0.0f });
	cloud->SetScale(6.0f);
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
