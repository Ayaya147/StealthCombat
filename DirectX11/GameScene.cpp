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
#include "NumberActor.h"
#include "MeshComponent.h"
#include "Mesh.h"
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
	mPlayer(new PlayerActor(this)),
	mFPS(nullptr)
{
	PlaneActor* plane = new PlaneActor(this);

	for (int i = 0; i < 5; i++)
	{
		EnemyActor* enemy = new EnemyActor(this);
	}
	for (int i = 0; i < 30; i++)
	{
	CloudActor* cloud = new CloudActor(this);
	}

	mMap = new Minimap(this);

	Actor* sprite = new Actor(this);
	Texture* tex = GetRenderer()->GetTexture("guide_keyboard");
	mGuideSprite = new SpriteComponent(sprite);
	mGuideSprite->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ -670.0f, 270.0f, 0.0f });
	sprite->SetScale(0.7f);

	sprite = new Actor(this);
	tex = GetRenderer()->GetTexture("marking");
	mMarkingSprite = new SpriteComponent(sprite, 110);
	mMarkingSprite->SetTexture(tex);
	mMarkingSprite->SetVisible(false);
	sprite->SetPosition(dx::XMFLOAT3{ -200.0f, 220.0f, 0.0f });
	sprite->SetScale(0.3f);

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

	sprite = new Actor(this);
	tex = GetRenderer()->GetTexture("ui_count");
	sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ -95.0f, -450.0f, 0.0f });
	sprite->SetScale(0.9f);

	mSpdNum = new NumberActor(this, 0, 4);
	mSpdNum->SetOriPosition(dx::XMFLOAT3{ -178.0f, -6.0f, 0.0f });
	mSpdNum->SetScale(0.6f);

	mOutCloudTime = new NumberActor(this, 0, 4);
	mOutCloudTime->SetOriPosition(dx::XMFLOAT3{ 242.0f, -6.0f, 0.0f });
	mOutCloudTime->SetScale(0.6f);

	mRestTime = new NumberActor(this, 300, 3);
	mRestTime->SetOriPosition(dx::XMFLOAT3{ 136.0f, -477.0f, 0.0f });
	mRestTime->SetScale(0.8f);

	mEnemyNum = new NumberActor(this, static_cast<float>(mEnemies.size()), 1);
	mEnemyNum->SetOriPosition(dx::XMFLOAT3{ 115.0f, -423.0f, 0.0f });
	mEnemyNum->SetScale(0.8f);

#ifdef DEBUG
	mFPS = new NumberActor(this, 0, 2);
	mFPS->SetOriPosition(dx::XMFLOAT3{ -900.0f, -500.0f, 0.0f });
	mFPS->SetScale(0.6f);
#endif
}

GameScene::~GameScene()
{
	delete mPhysWorld;
	delete mMap;
}

void GameScene::ProcessInput()
{
	BaseScene::ProcessInput();
}

void GameScene::Update()
{
	if (GetSceneState() == SceneState::EPlay)
	{
		mMap->Update(this);
		mOutCloudTime->SetValue(mPlayer->GetOutCloudTime() * 100.0f);
		mSpdNum->SetValue(mPlayer->GetForwardSpeed() * 160.0f);
		mEnemyNum->SetValue(static_cast<float>(mEnemies.size()));

		float restTime = mRestTime->GetValue() - GetDeltaTime();
		mRestTime->SetValue(restTime);
#ifdef DEBUG
		mFPS->SetValue(1.0f / GetDeltaTime());
#endif

		if (mPlayer->GetOutCloudTime() >= 12.0f ||
			mPlayer->GetForwardSpeed() < 800.0f / 160.f ||
			mEnemies.size() == 0 ||
			restTime <= 1.0f)
		{
			SetSceneState(SceneState::EQuit);
		}

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
	}

	BaseScene::Update();
}

void GameScene::GenerateOutput()
{
	BaseScene::GenerateOutput();

	GamePad* pad = GetInputSystem()->GetPad();
	Keyboard* keyboard = GetInputSystem()->GetKeyboard();
	if (pad->GetIsGamePad())
	{
		if (GetSceneState() == SceneState::EQuit ||
			pad->GetButtonState(XINPUT_GAMEPAD_RIGHT_THUMB) == ButtonState::EPressed)
		{
			pad->StopVibration();
			Parameter parameter;
			GetSceneManager()->ChangeScene(SceneManager::SceneType::EResult, parameter, true);
		}
	}
	else
	{
		if (GetSceneState() == SceneState::EQuit ||
			keyboard->GetKeyState(VK_RETURN) == ButtonState::EPressed)
		{
			Parameter parameter;
			GetSceneManager()->ChangeScene(SceneManager::SceneType::EResult, parameter, true);
		}
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

void GameScene::AddCloud(CloudActor* cloud)
{
	mClouds.emplace_back(cloud);
}

void GameScene::RemoveCloud(CloudActor* cloud)
{
	auto iter = std::find(mClouds.begin(), mClouds.end(), cloud);
	if (iter != mClouds.end())
	{
		std::iter_swap(iter, mClouds.end() - 1);
		mClouds.pop_back();
	}
}
