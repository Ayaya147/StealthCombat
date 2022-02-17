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
	mCloud(new CloudActor(this))
{
	for (int i = 0; i < 10; i++)
	{
		EnemyActor* enemy = new EnemyActor(this);
	}
	for (int i = 0; i < 50; i++)
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

	mCloudTimeNum = new NumberActor(this, 0, 4);
	mCloudTimeNum->SetOriPosition(dx::XMFLOAT3{ 242.0f, -6.0f, 0.0f });
	mCloudTimeNum->SetScale(0.6f);

	mSpdNum = new NumberActor(this, 0, 4);
	mSpdNum->SetOriPosition(dx::XMFLOAT3{ -178.0f, -6.0f, 0.0f });
	mSpdNum->SetScale(0.6f);

	mEnemyNum = new NumberActor(this, static_cast<float>(mEnemies.size()), 2);
	mEnemyNum->SetOriPosition(dx::XMFLOAT3{ 0.0f, -346.0f, 0.0f });
	mEnemyNum->SetScale(0.6f);
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
	mCloudTimeNum->SetValue(mPlayer->GetOutCloudTime() * 100.0f);
	mSpdNum->SetValue(mPlayer->GetForwardSpeed() * 160.0f);
	mEnemyNum->SetValue(static_cast<float>(mEnemies.size()));

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
	else if (
		pad->GetIsGamePad() &&
		pad->GetButtonState(XINPUT_GAMEPAD_RIGHT_THUMB) == ButtonState::EPressed)
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
