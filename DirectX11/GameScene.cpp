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

	renderer->SetAmbientLight(dx::XMFLOAT3{ 0.4f, 0.4f, 0.4f });
	DirectionalLightConstant dir = {};
	dir.mDirection = dx::XMFLOAT3{ 0.0f, -1.0f, -1.0f };
	dir.mDiffuseColor = dx::XMFLOAT3{ 0.8f, 0.9f, 1.0f };
	dir.mSpecColor = dx::XMFLOAT3{ 0.8f, 0.8f, 0.8f };
	renderer->SetDirectionalLight(dir);

	int n = 5;
	float size = 50.0f;
	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < n; i++)
		{
			PlaneActor* plane = new PlaneActor(this);
			float scale = plane->GetScale();
			plane->SetPosition(dx::XMFLOAT3{ -size * scale / 2.0f*(n - 1) + i * size* scale,0.0f,size* scale / 2.0f*(n - 1) - j * size* scale });
		}
	}


	PlayerActor* player = new PlayerActor(this, "player");
	CameraComponent* cc = new CameraComponent(player);
	player->SetPosition(dx::XMFLOAT3{ 0.0f,100.0f,0.0f });
	player->SetScale(0.1f);


	CloudActor* cloud = new CloudActor(this, "cube");
	cloud->SetPosition(dx::XMFLOAT3{ -2.0f,100.0f,0.0f });
	cloud->SetScale(0.25f);
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
