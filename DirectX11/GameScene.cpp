#include "GameScene.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "Renderer.h"
#include "PlayerActor.h"
#include "EnemyActor.h"
#include "MissileActor.h"
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

	mPlane = new PlaneActor(this);

	float height = 150.0f;

	PlayerActor* player = new PlayerActor(this);
	player->SetPosition(dx::XMFLOAT3{ 0.0f,height,0.0f });
	CameraComponent* cc = new CameraComponent(player);

	EnemyActor* enemy = new EnemyActor(this);
	enemy->SetPosition(dx::XMFLOAT3{ 0.0f,height,5.0f });
	
	MissileActor* missile = new MissileActor(this);
	missile->SetPosition(dx::XMFLOAT3{ 0.0f,height,-5.0f });

	CloudActor* cloud = new CloudActor(this);
	cloud->SetPosition(dx::XMFLOAT3{ 5.0f,height,0.0f });
	cloud->SetScale(6.0f);

	mCloud = new CloudActor(this);
	mCloud->SetPosition(dx::XMFLOAT3{ -5.0f,height,0.0f });
	//mCloud->SetScale(dx::XMFLOAT3{ 30.0f,10.0f,30.0f });
	mCloud->SetScale(10.0f);

	Actor* sprite = new Actor(this);
	sprite->SetTransformCBuffer(new TransformCBuffer(renderer, sprite));
	Texture* tex = renderer->GetTexture("Assets\\Texture\\minimap.png");
	SpriteComponent* sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ -670.0f, -270.0f, 0.0f });
	sprite->SetScale(0.6f);

	sprite = new Actor(this);
	sprite->SetTransformCBuffer(new TransformCBuffer(renderer, sprite));
	tex = renderer->GetTexture("Assets\\Texture\\guide.png");
	sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ -670.0f, 270.0f, 0.0f });
	sprite->SetScale(0.7f);
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

	if (GetInputSystem()->GetKeyboard()->KeyIsPressed(VK_RETURN))
	{
		Parameter parameter;
		mSceneManager->ChangeScene(SceneManager::SceneType::EResult, parameter, true);
	}
}
