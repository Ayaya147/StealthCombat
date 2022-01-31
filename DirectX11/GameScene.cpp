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
#include "SpriteComponent.h"
#include "Texture.h"
#include "InputSystem.h"
#include "Keyboard.h"

namespace dx = DirectX;

GameScene::GameScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	Renderer* renderer = GetRenderer();

	renderer->SetAmbientLight(dx::XMFLOAT3{ 0.15f, 0.15f, 0.15f });
	dx::XMFLOAT3 direction = dx::XMFLOAT3{ 0.0f, -1.0f, -1.0f };
	dx::XMFLOAT3 diffuseColor = dx::XMFLOAT3{ 0.8f, 0.9f, 1.0f };
	dx::XMFLOAT3 specColor = dx::XMFLOAT3{ 0.8f, 0.8f, 0.8f };
	renderer->SetDirectionalLight(direction, diffuseColor, specColor);

	PlaneActor* plane = new PlaneActor(this);

	PlayerActor* player = new PlayerActor(this);
	player->SetPosition(dx::XMFLOAT3{ 0.0f,100.0f,0.0f });
	player->SetScale(0.1f);
	CameraComponent* cc = new CameraComponent(player);
	
	CloudActor* cloud = new CloudActor(this);
	cloud->SetPosition(dx::XMFLOAT3{ 5.0f,100.0f,0.0f });
	cloud->SetScale(4.0f);

	mCloud = new CloudActor(this);
	mCloud->SetPosition(dx::XMFLOAT3{ -5.0f,100.0f,0.0f });
	//mCloud->SetScale(dx::XMFLOAT3{ 10.0f,10.0f,10.0f });
	mCloud->SetScale(10.0f);

	Actor* sprite = new Actor(this);
	sprite->SetTransformCBuffer(new TransformCBuffer(renderer, sprite));
	Texture* tex = renderer->GetTexture("Assets\\Texture\\test.png");
	SpriteComponent* sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ -700.0f, -400.0f, 0.0f });
	sprite->SetScale(0.5f);

	sprite = new Actor(this);
	sprite->SetTransformCBuffer(new TransformCBuffer(renderer, sprite));
	tex = renderer->GetTexture("Assets\\Texture\\normal1.png");
	sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 0.0f, -400.0f, 0.0f });
	sprite->SetScale(0.2f);
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

	if (GetInputSystem()->GetKeyboard()->KeyIsPressed('V'))
	{
		Parameter parameter;
		mSceneManager->ChangeScene(SceneManager::SceneType::ETitle, parameter, true);
	}
}
