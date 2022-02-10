#include "GameScene.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "Renderer.h"
#include "PlayerActor.h"
#include "EnemyActor.h"
#include "MissileActor.h"
#include "CloudActor.h"
#include "PlaneActor.h"
#include "NumberActor.h"
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "Mesh.h"
#include "TransformCBuffer.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "InputSystem.h"
#include "GamePad.h"
#include "Keyboard.h"
#include "Random.h"
#include "PhysWorld.h"

namespace dx = DirectX;

GameScene::GameScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter),
	mPhysWorld(new PhysWorld(this))
{
	Renderer* renderer = GetRenderer();

	mPlane = new PlaneActor(this);

	float height = 150.0f;

	mPlayer = new PlayerActor(this);
	mPlayer->SetPosition(dx::XMFLOAT3{ 0.0f,height,0.0f });
	CameraComponent* cc = new CameraComponent(mPlayer);
	cc->SnapToIdeal();

	EnemyActor* enemy = new EnemyActor(this);
	enemy->SetPosition(dx::XMFLOAT3{ 0.0f,height,5.0f });
	
	MissileActor* missile = new MissileActor(this);
	missile->SetPosition(dx::XMFLOAT3{ 0.0f,height,-5.0f });

	//for (int i = 0; i < 50; i++)
	//{
	//	CloudActor* cloud = new CloudActor(this);
	//	cloud->SetPosition(dx::XMFLOAT3{ 5.0f + i * 2.0f,height,i*2.0f });
	//	//cloud->SetPosition(dx::XMFLOAT3{ Random::GetFloatRange(-250.0f,250.0f),height,Random::GetFloatRange(-250.0f,250.0f) });
	//	cloud->SetScale(5.0f);
	//}

	mCloud = new CloudActor(this);
	mCloud->SetPosition(dx::XMFLOAT3{ 0.0f,height,0.0f });
	mCloud->SetScale(dx::XMFLOAT3{ 100.0f,25.0f,100.0f });
	//mCloud->SetScale(10.0f);

	Actor* sprite = new Actor(this);
	Texture* tex = renderer->GetTexture("Assets\\Texture\\minimap.png");
	SpriteComponent* sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ -670.0f, -270.0f, 0.0f });
	sprite->SetScale(0.6f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("Assets\\Texture\\speed.png");
	sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ -200.0f, 20.0f, 0.0f });
	sprite->SetScale(0.6f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("Assets\\Texture\\time.png");
	sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ 200.0f, 20.0f, 0.0f });
	sprite->SetScale(0.6f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("Assets\\Texture\\guide_keyboard.png");
	mSprite = new SpriteComponent(sprite);
	mSprite->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ -670.0f, 270.0f, 0.0f });
	sprite->SetScale(0.7f);

	NumberActor* number = new NumberActor(this, 0, 4);
	number->SetOriPosition(dx::XMFLOAT3{ -180.0f, -6.0f, 0.0f });
}

GameScene::~GameScene()
{
	delete mPhysWorld;
}

void GameScene::ProcessInput()
{
	BaseScene::ProcessInput();
}

void GameScene::Update()
{
	GamePad* pad = GetInputSystem()->GetPad();
	if (pad->GetIsGamePad())
	{
		Texture* tex = GetRenderer()->GetTexture("Assets\\Texture\\guide_pad.png");
		mSprite->SetTexture(tex);
	}
	else
	{
		Texture* tex = GetRenderer()->GetTexture("Assets\\Texture\\guide_keyboard.png");
		mSprite->SetTexture(tex);
	}
	
	BaseScene::Update();
}

void GameScene::GenerateOutput()
{
	BaseScene::GenerateOutput();

	GamePad* pad = GetInputSystem()->GetPad();
	Keyboard* keyboard = GetInputSystem()->GetKeyboard();

	if (!pad->GetIsGamePad() && keyboard->GetKeyState(VK_RETURN) == ButtonState::EPressed)
	{
		Parameter parameter;
		mSceneManager->ChangeScene(SceneManager::SceneType::EResult, parameter, true);
	}
	else if (pad->GetIsGamePad() && pad->GetButtonState(XINPUT_GAMEPAD_RIGHT_THUMB) == ButtonState::EPressed)
	{
		Parameter parameter;
		mSceneManager->ChangeScene(SceneManager::SceneType::EResult, parameter, true);
	}
}
