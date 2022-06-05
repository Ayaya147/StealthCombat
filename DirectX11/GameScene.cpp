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
#include "Mesh.h"
#include "SpriteComponent.h"
#include "NumberSpriteComponent.h"
#include "Texture.h"
#include "InputSystem.h"
#include "AudioSystem.h"
#include "GamePad.h"
#include "Keyboard.h"
#include "Random.h"
#include "PhysWorld.h"
#include "Minimap.h"
#include "XMFloatHelper.h"
#include "Fade.h"
#include "PauseScreen.h"
#include "ParticleManager.h"
#include "TranslucenceComponent.h"

//#define FPS_ENABLE
namespace dx = DirectX;
static SceneManager::SceneType gNextScene = SceneManager::SceneType::ETitle;

GameScene::GameScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter),
	mPhysWorld(new PhysWorld(this)),
	mPlayer(new PlayerActor(this)),
	mWin(false),
	mIsCautionSE(false),
	mQuitTime(1.5f),
	mDestroyedSpriteTime(0.0f)
{
	SetParticleManager(new ParticleManager(this, GetRenderer()));
	GetParticleManager()->CreateParticleSystem(GetRenderer());

	gNextScene = SceneManager::SceneType::ETitle;
	GetRenderer()->ResetLight();

	CreateGameActor();
	CreateNumberActor();
	CreateUIActor();
	CreatePauseScreen();
	CreateSound();
}

GameScene::~GameScene()
{
	GetAudioSystem()->StopSoundAll();
	delete mPhysWorld;
	delete mMap;
}

void GameScene::ProcessInput()
{
	switch (GetSceneState())
	{
	case SceneState::EPlay:
		if (GetInputSystem()->GetScenePause())
		{
			GetInputSystem()->GetPad()->StopVibration();
			SetSceneState(SceneState::EPaused);
			mBackgroundUI->SetUIState(PauseScreen::UIState::EActive);
			mMenuUI->SetUIState(PauseScreen::UIState::EActive);

			int index = GetAudioSystem()->LoadSound("se_ok");
			GetAudioSystem()->PlaySoundEx(index, 0);
		}
		break;

	case SceneState::EPaused:
		if (GetInputSystem()->GetX())
		{
			SetSceneState(SceneState::EPlay);
			for (auto ui : GetPauseUIStack())
			{
				ui->SetUIState(PauseScreen::UIState::EClosing);
			}

			int index = GetAudioSystem()->LoadSound("se_ok");
			GetAudioSystem()->PlaySoundEx(index, 0);
		}
		else if (GetInputSystem()->GetY())
		{
			SetSceneState(SceneState::EQuit);
			for (auto ui : GetPauseUIStack())
			{
				ui->SetUIState(PauseScreen::UIState::EClosing);
			}
			mQuitTime = 0.0f;

			int index = GetAudioSystem()->LoadSound("se_ok");
			GetAudioSystem()->PlaySoundEx(index, 0);
		}
		break;

	case SceneState::EGameEnd:
		if (mQuitTime <= 0.0f && GetInputSystem()->GetY())
		{
			SetSceneState(SceneState::EQuit);
			gNextScene = SceneManager::SceneType::ETitle;
			for (auto ui : GetPauseUIStack())
			{
				ui->SetUIState(PauseScreen::UIState::EClosing);
			}

			int index = GetAudioSystem()->LoadSound("se_ok");
			GetAudioSystem()->PlaySoundEx(index, 0);
		}
		else if (mQuitTime <= 0.0f && GetInputSystem()->GetX() && !mWin)
		{
			SetSceneState(SceneState::EQuit);
			gNextScene = SceneManager::SceneType::EGame;
			for (auto ui : GetPauseUIStack())
			{
				ui->SetUIState(PauseScreen::UIState::EClosing);
			}

			int index = GetAudioSystem()->LoadSound("se_ok");
			GetAudioSystem()->PlaySoundEx(index, 0);
		}
		break;
	}

	BaseScene::ProcessInput();
}

void GameScene::Update()
{
	switch (GetSceneState())
	{
	case SceneState::EPlay:
	{
		GetParticleManager()->Update(GetRenderer());
		mMap->Update(this);
		mOutCloudTime->SetValue(mPlayer->GetOutCloudTime() * 100.0f);
		mSpdNum->SetValue(mPlayer->GetForwardSpeed() * 160.0f);
		mEnemyNum->SetValue(static_cast<float>(mEnemies.size()));

#ifdef  FPS_ENABLE
		mFPS->SetValue(10.0f / GetDeltaTime());
#endif

		float restTime = mRestTime->GetValue() - GetDeltaTime();
		mRestTime->SetValue(restTime);

		if (mEnemies.size() == 0)
		{
			SetSceneState(SceneState::EGameEnd);
			mWin = true;
		}
		else if (restTime <= 1.0f)
		{
			SetSceneState(SceneState::EGameEnd);
		}

		if (restTime <= 10.0f)
		{
			mCautionGameTime->SetVisible(true);
			mRestTime->GetNumberSpriteComp()->SetColor(dx::XMFLOAT4{ 100.0f,0.0f,0.0f,1.0f });
			mEnemyNum->GetNumberSpriteComp()->SetColor(dx::XMFLOAT4{ 100.0f,0.0f,0.0f,1.0f });
			mUICountSprite->SetColor(dx::XMFLOAT4{ 100.0f,0.0f,0.0f,1.0f });

			if (!mIsCautionSE)
			{
				GetAudioSystem()->PlaySoundEx(mCautionSE, XAUDIO2_LOOP_INFINITE);
				mIsCautionSE = true;
			}
		}
		else
		{
			mCautionGameTime->SetVisible(false);
			mRestTime->GetNumberSpriteComp()->SetColor(dx::XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f });
			mEnemyNum->GetNumberSpriteComp()->SetColor(dx::XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f });
			mUICountSprite->SetColor(dx::XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f });
		}

		if (mPlayer->GetOutCloudTime() <= 4.0f)
		{
			mCautionCloudTime->SetVisible(true);
			mOutCloudTime->GetNumberSpriteComp()->SetColor(dx::XMFLOAT4{ 100.0f,0.0f,0.0f,1.0f });
			mTimeSprite->SetColor(dx::XMFLOAT4{ 100.0f,0.0f,0.0f,1.0f });

			if (!mIsCautionSE)
			{
				GetAudioSystem()->PlaySoundEx(mCautionSE, XAUDIO2_LOOP_INFINITE);
				mIsCautionSE = true;
			}
		}
		else
		{
			mCautionCloudTime->SetVisible(false);
			mOutCloudTime->GetNumberSpriteComp()->SetColor(dx::XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f });
			mTimeSprite->SetColor(dx::XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f });

			if (mIsCautionSE && restTime > 10.0f)
			{
				GetAudioSystem()->StopSound(mCautionSE);
				mIsCautionSE = false;
			}
		}

		if (mPlayer->GetOutCloudTime() <= 0.0f && !mPlayer->GetIsLockedOn())
		{
			dx::XMFLOAT3 pos = mPlayer->GetPosition() + mPlayer->GetForward() * 60.0f;
			MissileActor* missile = new MissileActor(this, mPlayer, pos, 25.0f);
			mPlayer->SetLockedOn(true);

			int index = GetAudioSystem()->LoadSound("se_missile");
			GetAudioSystem()->PlaySoundEx(index, 0);
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

		if (mDestroyedSpriteTime > 0.0f)
		{
			mDestroyedSpriteTime -= GetDeltaTime();
			mDestroyedSprite->SetVisible(true);
		}
		else
		{
			mDestroyedSprite->SetVisible(false);
		}

		for (int i = 0; i < mEnemyGuideTranslucenceComps.size(); i++)
		{
			if (mEnemies[i]->GetIsInCloud())
			{
				mEnemyGuideTranslucenceComps[i]->SetVisible(false);
				continue;
			}
			else
			{
				mEnemyGuideTranslucenceComps[i]->SetVisible(true);
			}

			float dist = 3.0f;
			dx::XMFLOAT3 playerToEnemy = DXMath::Normalize(
				mEnemies[i]->GetPosition() - mPlayer->GetPosition()
			);
			mEnemyGuideTranslucenceComps[i]->GetOwner()->SetPosition(
				mPlayer->GetPosition() + playerToEnemy * dist
			);
			mEnemyGuideTranslucenceComps[i]->GetOwner()->SetRotation({ mEnemies[i]->GetRotation() });
		}
	}
		break;

	case SceneState::EGameEnd:
		mQuitTime -= GetDeltaTime();
		if (mQuitTime <= 0.0f)
		{
			mBackgroundUI->SetUIState(PauseScreen::UIState::EActive);

			if (mWin)
			{
				mVictoryUI->SetUIState(PauseScreen::UIState::EActive);
			}
			else
			{
				mDefeatUI->SetUIState(PauseScreen::UIState::EActive);
			}
		}
		break;
	}

	BaseScene::Update();
}

void GameScene::GenerateOutput()
{
	BaseScene::GenerateOutput();

	switch (GetSceneState())
	{
	case SceneState::EPlay:
	{
		GamePad* pad = GetInputSystem()->GetPad();
		pad->SetVibration(mVibrationStrength);
		GetAudioSystem()->SetVolume(mGameBGM, 1.0f);
	}
		break;

	case SceneState::EPaused:
	case SceneState::EGameEnd:
		GetInputSystem()->GetPad()->StopVibration();
		GetAudioSystem()->SetVolume(mGameBGM, 0.0f);
		GetAudioSystem()->SetVolume(mAirplaneBGM, 0.0f);
		GetAudioSystem()->StopSound(mCautionSE);
		GetAudioSystem()->StopSound(mLockOnSE);
		mIsCautionSE = false;
		mPlayer->SetIsLockOnSE(false);
		break;

	case SceneState::EQuit:
		GetFade()->SetFadeState(Fade::FadeState::EFadeOut);

		if (GetFade()->GetAlpha() >= 1.0f)
		{
			Parameter parameter;
			GetSceneManager()->ChangeScene(SceneManager::SceneType::EResult, parameter, true);
		}
		break;
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

void GameScene::RemoveEnemyGuideTranslucenceComp()
{
	delete mEnemyGuideTranslucenceComps.back();
	mEnemyGuideTranslucenceComps.pop_back();
}

void GameScene::CreateGameActor()
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

	Mesh* mesh = GetRenderer()->GetMesh("planeEnemy");
	for (int i = 0; i < mEnemies.size(); i++)
	{
		Actor* actor = new Actor(this);
		mesh->ParsePlaneMesh(GetRenderer(), "enemy_guide", L"Phong", 2, 0.5f, true);
		mEnemyGuideTranslucenceComps.emplace_back(new TranslucenceComponent(actor, mesh, 400));
	}
}

void GameScene::CreateNumberActor()
{
	mSpdNum = new NumberActor(this, 0, 4);
	mSpdNum->SetOriPosition(dx::XMFLOAT3{ -178.0f, 44.0f, 0.0f });
	mSpdNum->SetScale(0.6f);

	mOutCloudTime = new NumberActor(this, 0, 4);
	mOutCloudTime->SetOriPosition(dx::XMFLOAT3{ 242.0f, 44.0f, 0.0f });
	mOutCloudTime->SetScale(0.6f);

	mRestTime = new NumberActor(this, 240, 3);
	mRestTime->SetOriPosition(dx::XMFLOAT3{ 126.0f, -477.0f, 0.0f });
	mRestTime->SetScale(0.8f);

	mEnemyNum = new NumberActor(this, static_cast<float>(mEnemies.size()), 1);
	mEnemyNum->SetOriPosition(dx::XMFLOAT3{ 105.0f, -423.0f, 0.0f });
	mEnemyNum->SetScale(0.8f);

#ifdef  FPS_ENABLE
	mFPS = new NumberActor(this, 0.0f, 3);
	mFPS->SetOriPosition(dx::XMFLOAT3{ -890.0f, -500.0f, 0.0f });
	mFPS->SetScale(0.5f);
#endif
}

void GameScene::CreateUIActor()
{
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
	sprite->SetScale(0.27f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("marking");
	mMarkingPlayerSprite = new SpriteComponent(sprite, tex, 110);
	mMarkingPlayerSprite->SetVisible(false);
	sprite->SetScale(0.27f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("speed");
	SpriteComponent* sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ -200.0f, 70.0f, 0.0f });
	sprite->SetScale(0.6f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("time");
	mTimeSprite = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 200.0f, 70.0f, 0.0f });
	sprite->SetScale(0.6f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("ui_count");
	mUICountSprite = new SpriteComponent(sprite, tex);
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

	sprite = new Actor(this);
	tex = renderer->GetTexture("destroyed");
	mDestroyedSprite = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 0.0f, -140.0f, 0.0f });
	sprite->SetScale(0.55f);
	mCautionCloudTime->SetVisible(false);

	mMap = new Minimap(this);
}

void GameScene::CreatePauseScreen()
{
	Renderer* renderer = GetRenderer();

	Texture* tex = renderer->GetTexture("fade");
	mBackgroundUI = new PauseScreen(this, tex);
	mBackgroundUI->SetScale(40.0f);
	mBackgroundUI->ComputeWorldTransform();
	mBackgroundUI->SetColor(dx::XMFLOAT4{ 1.0f,1.0f,1.0f,0.7f });

	tex = renderer->GetTexture("menu");
	mMenuUI = new PauseScreen(this, tex);
	mMenuUI->SetScale(0.8f);
	mMenuUI->ComputeWorldTransform();

	tex = renderer->GetTexture("victory");
	mVictoryUI = new PauseScreen(this, tex);
	mVictoryUI->SetScale(1.0f);
	mVictoryUI->ComputeWorldTransform();

	tex = renderer->GetTexture("defeat");
	mDefeatUI = new PauseScreen(this, tex);
	mDefeatUI->SetScale(1.0f);
	mDefeatUI->ComputeWorldTransform();
}

void GameScene::CreateSound()
{
	AudioSystem* audio = GetAudioSystem();
	mGameBGM = audio->LoadSound("bgm_game");
	audio->PlaySoundEx(mGameBGM, XAUDIO2_LOOP_INFINITE);

	mAirplaneBGM = audio->LoadSound("bgm_airplane");
	audio->PlaySoundEx(mAirplaneBGM, XAUDIO2_LOOP_INFINITE);
	audio->SetVolume(mAirplaneBGM, 0.5f);

	mCautionSE = audio->LoadSound("se_caution");
	mLockOnSE = audio->LoadSound("se_lockOn");
}
