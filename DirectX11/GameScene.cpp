#include "GameScene.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "Renderer.h"
#include "Actor.h"
#include "PlayerActor.h"
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

	Actor* camera = new Actor(this);
	camera->SetPosition(dx::XMFLOAT3{ 0.0f,0.0f,-5.0f });
	CameraComponent* cc = new CameraComponent(camera);

	PlayerActor* player = new PlayerActor(this, "Assets\\Models\\suzanne.obj");
	player->SetPosition(dx::XMFLOAT3{ 0.0f,0.0f,2.0f });
	player = new PlayerActor(this, "Assets\\Models\\test.obj");
	player->SetPosition(dx::XMFLOAT3{ 2.0f,0.0f,0.0f });
	player = new PlayerActor(this, "Assets\\Models\\test.obj");
	player->SetPosition(dx::XMFLOAT3{ -2.0f,0.0f,0.0f });

	Actor* plane = new Actor(this);
	plane->SetScale(1.0f);
	plane->SetPosition(dx::XMFLOAT3{ 0.0f,-1.0f,0.0f });
	plane->SetTransformCBuffer(new TransformCBuffer(renderer, plane));
	Mesh* mesh = renderer->GetMesh("Assets\\Models\\plane.obj", L"Phong");
	MeshComponent* mc = new MeshComponent(plane, mesh);

	renderer->SetAmbientLight(dx::XMFLOAT3{ 0.2f, 0.2f, 0.2f });
	DirectionalLight dir = {};
	dir.mDirection = dx::XMFLOAT3{ 0.0f, -1.0f, -1.0f };
	dir.mDiffuseColor = dx::XMFLOAT3{ 0.8f, 0.8f, 0.8f };
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
