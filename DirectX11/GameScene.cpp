#include "GameScene.h"
#include <algorithm>
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
	CameraComponent* cc = new CameraComponent(mPlayer);
	cc->SnapToIdeal();

	for (int i = 0; i < 50; i++)
	{
		EnemyActor* enemy = new EnemyActor(this);
	}	

	for (int i = 0; i < 50; i++)
	{
		CloudActor* cloud = new CloudActor(this);
	}

	mCloud = new CloudActor(this);

	Actor* sprite = new Actor(this);
	Texture* tex = renderer->GetTexture("Assets\\Texture\\minimap.png");
	SpriteComponent* sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ -670.0f, -270.0f, 0.0f });
	sprite->SetScale(0.6f);

	mDirectionMinimap = new Actor(this);
	tex = renderer->GetTexture("Assets\\Texture\\minimap_direction.png");
	sc = new SpriteComponent(mDirectionMinimap);
	sc->SetTexture(tex);
	mDirectionMinimap->SetPosition(dx::XMFLOAT3{ -670.0f, -270.0f, 0.0f });
	mDirectionMinimap->SetScale(0.55f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("Assets\\Texture\\minimap_player.png");
	sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ -670.0f, -270.0f, 0.0f });
	sprite->SetScale(0.1f);

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
	mDirectionMinimap->SetRotation(dx::XMFLOAT3{ 0.0f,0.0f,mPlayer->GetRotation().y });

	auto func = [](EnemyActor* enemy1, EnemyActor* enemy2)
	{
		return enemy1->GetDistFromPlayer() < enemy2->GetDistFromPlayer();
	};
	std::sort(mEnemies.begin(), mEnemies.end(), func);


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

void GameScene::AddEnemy(EnemyActor* enemy)
{
	mEnemies.emplace_back(enemy);
}

void GameScene::RemoveEnemy(EnemyActor* enemy)
{
	auto iter = std::find(mEnemies.begin(), mEnemies.end(), enemy);
	if (iter != mEnemies.end())
	{
		std::iter_swap(iter, mEnemies.end() - 1);
		mEnemies.pop_back();
	}
}
