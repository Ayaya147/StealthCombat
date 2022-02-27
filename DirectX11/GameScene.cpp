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
#include "XMFloatHelper.h"

namespace dx = DirectX;

GameScene::GameScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter),
	mPhysWorld(new PhysWorld(this)),
	mPlayer(new PlayerActor(this)),
	mFPS(nullptr),
	mIsMissile(false)
{
	PlaneActor* plane = new PlaneActor(this);

	for (int i = 0; i < 5; i++)
	{
		EnemyActor* enemy = new EnemyActor(this);
	}
	for (int i = 0; i < 28; i++)
	{
		CloudActor* cloud = new CloudActor(this);
	}

	mMap = new Minimap(this);

	Renderer* renderer = GetRenderer();
	Actor* sprite = new Actor(this);
	Texture* tex = renderer->GetTexture("guide_keyboard");
	mGuideSprite = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ -670.0f, 270.0f, 0.0f });
	sprite->SetScale(0.7f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("marking");
	mMarkingEnemySprite = new SpriteComponent(sprite, tex, 110);
	mMarkingEnemySprite->SetVisible(false);
	sprite->SetScale(0.3f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("marking");
	mMarkingPlayerSprite = new SpriteComponent(sprite, tex, 110);
	mMarkingPlayerSprite->SetVisible(false);
	sprite->SetScale(0.3f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("speed");
	SpriteComponent* sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ -200.0f, 70.0f, 0.0f });
	sprite->SetScale(0.6f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("time");
	sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 200.0f, 70.0f, 0.0f });
	sprite->SetScale(0.6f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("ui_count");
	sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ -95.0f, -450.0f, 0.0f });
	sprite->SetScale(0.9f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("caution");
	mCautionGameTime = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 0.0f, -476.0f, 0.0f });
	sprite->SetScale(0.5f);
	mCautionGameTime->SetVisible(false);

	sprite = new Actor(this);
	tex = renderer->GetTexture("caution");
	mCautionCloudTime = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 205.0f, -12.0f, 0.0f });
	sprite->SetScale(0.5f);
	mCautionCloudTime->SetVisible(false);

	mSpdNum = new NumberActor(this, 0, 4);
	mSpdNum->SetOriPosition(dx::XMFLOAT3{ -178.0f, 44.0f, 0.0f });
	mSpdNum->SetScale(0.6f);

	mOutCloudTime = new NumberActor(this, 0, 4);
	mOutCloudTime->SetOriPosition(dx::XMFLOAT3{ 242.0f, 44.0f, 0.0f });
	mOutCloudTime->SetScale(0.6f);

	mRestTime = new NumberActor(this, 300, 3);
	mRestTime->SetOriPosition(dx::XMFLOAT3{ 126.0f, -477.0f, 0.0f });
	mRestTime->SetScale(0.8f);

	mEnemyNum = new NumberActor(this, static_cast<float>(mEnemies.size()), 1);
	mEnemyNum->SetOriPosition(dx::XMFLOAT3{ 105.0f, -423.0f, 0.0f });
	mEnemyNum->SetScale(0.8f);

	mFPS = new NumberActor(this, 0, 2);
	mFPS->SetOriPosition(dx::XMFLOAT3{ -900.0f, -500.0f, 0.0f });
	mFPS->SetScale(0.6f);

	renderer->ResetLight();
}

GameScene::~GameScene()
{
	delete mPhysWorld;
	delete mMap;
}

void GameScene::ProcessInput()
{
	if (GetInputSystem()->GetSceneChangeEnter())
	{
		SetSceneState(SceneState::EQuit);
	}

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
		mFPS->SetValue(1.0f / GetDeltaTime());

		float restTime = mRestTime->GetValue() - GetDeltaTime();
		mRestTime->SetValue(restTime);

		if (mEnemies.size() == 0 ||	restTime <= 1.0f)
		{
			SetSceneState(SceneState::EQuit);
		}

		if (restTime <= 10.0f)
		{
			mCautionGameTime->SetVisible(true);
		}
		else
		{
			mCautionGameTime->SetVisible(false);
		}

		if (mPlayer->GetOutCloudTime() >= 10.0f)
		{
			mCautionCloudTime->SetVisible(true);
		}
		else
		{
			mCautionCloudTime->SetVisible(false);
		}

		if (mPlayer->GetOutCloudTime() >= 14.0f && !mIsMissile)
		{
			dx::XMFLOAT3 pos = mPlayer->GetPosition() + mPlayer->GetForward() * 60.0f;
			MissileActor* missile = new MissileActor(this, mPlayer, pos, 25.0f);
			mPlayer->SetLockedOn(true);
			mIsMissile = true;
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

	if (GetSceneState() == SceneState::EQuit)
	{
		GetInputSystem()->GetPad()->StopVibration();
		Parameter parameter;
		GetSceneManager()->ChangeScene(SceneManager::SceneType::EResult, parameter, true);
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
