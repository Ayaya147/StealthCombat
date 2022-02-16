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
#include "Mesh.h"
#include "TransformCBuffer.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "InputSystem.h"
#include "GamePad.h"
#include "Keyboard.h"
#include "Random.h"
#include "PhysWorld.h"
#include "Minimap.h"

namespace dx = DirectX;

GameScene::GameScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter),
	mPhysWorld(new PhysWorld(this)),
	mPlane(new PlaneActor(this)),
	mPlayer(new PlayerActor(this)),
	mCloud(new CloudActor(this)),
	mMap(new Minimap(this))
{
	for (int i = 0; i < 50; i++)
	{
		EnemyActor* enemy = new EnemyActor(this);
	}
	for (int i = 0; i < 50; i++)
	{
		CloudActor* cloud = new CloudActor(this);
	}

	Actor* sprite = new Actor(this);
	Texture* tex = GetRenderer()->GetTexture("guide_keyboard");
	mGuideSprite = new SpriteComponent(sprite);
	mGuideSprite->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ -670.0f, 270.0f, 0.0f });
	sprite->SetScale(0.7f);

	sprite = new Actor(this);
	tex = GetRenderer()->GetTexture("speed");
	SpriteComponent* sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ -200.0f, 20.0f, 0.0f });
	sprite->SetScale(0.6f);

	sprite = new Actor(this);
	tex = GetRenderer()->GetTexture("time");
	sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ 200.0f, 20.0f, 0.0f });
	sprite->SetScale(0.6f);
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
	mMap->Update(this);

	GamePad* pad = GetInputSystem()->GetPad();
	if (pad->GetIsGamePad())
	{
		Texture* tex = GetRenderer()->GetTexture("guide_pad");
		mGuideSprite->SetTexture(tex);
	}
	else
	{
		Texture* tex = GetRenderer()->GetTexture("guide_keyboard");
		mGuideSprite->SetTexture(tex);
	}

	auto func = [](EnemyActor* enemy1, EnemyActor* enemy2)
	{
		return enemy1->GetDistFromPlayer() < enemy2->GetDistFromPlayer();
	};
	std::sort(mEnemies.begin(), mEnemies.end(), func);
	
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
