#include "GameScene.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "Renderer.h"
#include "Actor.h"
#include "PlayerActor.h"
#include "PlaneActor.h"
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "Mesh.h"
#include "TransformCBuffer.h"

namespace dx = DirectX;

GameScene::GameScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	Renderer* renderer = GetSceneManager()->GetRenderer();

	mCamera = new Actor(this);
	mCamera->SetPosition(dx::XMFLOAT3{ 0.0f,15.0f,0.0f });
	CameraComponent* cc = new CameraComponent(mCamera);

	//PlayerActor* player = new PlayerActor(this, "slime");
	//player->SetPosition(dx::XMFLOAT3{ -2.0f,0.0f,0.0f });
	//player->SetScale(5.0f);
	//player = new PlayerActor(this, "test");
	//player->SetPosition(dx::XMFLOAT3{ 2.0f,0.0f,0.0f });
	//player->SetScale(2.0f);

	PlaneActor* plane = new PlaneActor(this);
	plane->SetPosition(dx::XMFLOAT3{ 0.0f,0.0f,0.0f });

	plane = new PlaneActor(this);
	plane->SetPosition(dx::XMFLOAT3{ 5.0f,0.0f,0.0f });
	plane = new PlaneActor(this);
	plane->SetPosition(dx::XMFLOAT3{ -5.0f,0.0f,0.0f });
	plane = new PlaneActor(this);
	plane->SetPosition(dx::XMFLOAT3{ 0.0f,0.0f,5.0f });
	plane = new PlaneActor(this);
	plane->SetPosition(dx::XMFLOAT3{ 0.0f,0.0f,-5.0f });
	plane = new PlaneActor(this);
	plane->SetPosition(dx::XMFLOAT3{ 5.0f,0.0f,5.0f });

	renderer->SetAmbientLight(dx::XMFLOAT3{ 0.4f, 0.4f, 0.4f });
	DirectionalLight dir = {};
	dir.mDirection = dx::XMFLOAT3{ 0.0f, -1.0f, -1.0f };
	dir.mDiffuseColor = dx::XMFLOAT3{ 0.8f, 0.9f, 1.0f };
	dir.mSpecColor = dx::XMFLOAT3{ 0.8f, 0.8f, 0.8f };
	renderer->SetDirectionalLight(dir);
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

	//dx::XMFLOAT3 pos = mCamera->GetPosition();
	//pos.z = pos.z + mSpeed * 0.016f;

	//if (pos.z >= 0)
	//{
	//	mSpeed *= -1;
	//	pos.z = -0.001f;
	//}
	//else if (pos.z <= -15.0f)
	//{
	//	mSpeed *= -1;
	//}

	//mCamera->SetPosition(pos);
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
