#include "TitleScene.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "Actor.h"
#include "InputSystem.h"
#include "Keyboard.h"
#include "GamePad.h"
#include "Fade.h"
#include "PlaneActor.h"
#include "CloudActor.h"
#include "DefineConstant.h"
#include "CameraComponent.h"
#include "AudioSystem.h"
#include "ParticleManager.h"

namespace dx = DirectX;

TitleScene::TitleScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter),
	mIsTutorial(false),
	mAlpha(1.0f),
	mRate(-0.8f)
{
	Renderer* renderer = GetRenderer();

	SetParticleManager(new ParticleManager(this, renderer));
	GetParticleManager()->CreateParticleSystem(renderer);

	PlaneActor* plane = new PlaneActor(this);
	CloudActor* cloud = new CloudActor(this);
	cloud->SetScale(dx::XMFLOAT3{ 20.0f,5.0f,20.0f });
	cloud->SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,1.5f });

	Actor* actor = new Actor(this);
	actor->SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,0.0f });
	CameraComponent* cc = new CameraComponent(actor, CameraComponent::CameraType::ENormal);
	cc->SnapToIdeal();

	Actor* sprite = new Actor(this);
	Texture* tex = renderer->GetTexture("fade");
	SpriteComponent* sc = new SpriteComponent(sprite, tex);
	sc->SetColor(dx::XMFLOAT4{ 1.0f,1.0f,1.0f,0.7f });
	sprite->SetScale(40.0f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("mode");
	mModeSprite = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 25.0f, 300.0f, 0.0f });
	sprite->SetScale(0.5f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("title_01");
	mTitle1Sprite = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ -278.0f, -46.0f, 0.0f });
	sprite->SetScale(1.0f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("title_02");
	mTitle2Sprite = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 200.0f, 0.0f, 0.0f });
	sprite->SetScale(1.0f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("back");
	mBackSprite = new SpriteComponent(sprite, tex);
	mBackSprite->SetVisible(false);
	sprite->SetPosition(dx::XMFLOAT3{ 750.0f, 470.0f, 0.0f });
	sprite->SetScale(0.4f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("tutorial");
	mTutorialSprite = new SpriteComponent(sprite, tex);
	mTutorialSprite->SetVisible(false);
	sprite->SetScale(1.0f);

	renderer->ResetLight();

	if (!parameter.GetIsFromDemo())
	{
		AudioSystem* audio = GetAudioSystem();
		int idx = audio->LoadSound("bgm_title");
		audio->PlaySoundEx(idx, XAUDIO2_LOOP_INFINITE);
		audio->SetVolume(idx, 0.2f);
	}
}

TitleScene::~TitleScene()
{
}

void TitleScene::ProcessInput()
{
	if (GetSceneState() == SceneState::EPlay)
	{
		if (!mIsTutorial)
		{
			if (GetInputSystem()->GetX())
			{
				SetSceneState(SceneState::EQuit);
				SetNextScene(SceneType::EGame);

				int index = GetAudioSystem()->LoadSound("se_ok");
				GetAudioSystem()->PlaySoundEx(index, 0);
			}
			else if (GetInputSystem()->GetY())
			{
				SetSceneState(SceneState::EQuit);
				SetNextScene(SceneType::EDemo);

				int index = GetAudioSystem()->LoadSound("se_ok");
				GetAudioSystem()->PlaySoundEx(index, 0);
			}
			else if (GetInputSystem()->GetB())
			{
				mIsTutorial = true;
				mTitle1Sprite->SetVisible(false);
				mTitle2Sprite->SetVisible(false);
				mModeSprite->SetVisible(false);
				mBackSprite->SetVisible(true);
				mTutorialSprite->SetVisible(true);

				int index = GetAudioSystem()->LoadSound("se_ok");
				GetAudioSystem()->PlaySoundEx(index, 0);
			}
			else if (GetInputSystem()->GetA())
			{
				PostQuitMessage(0);
			}
		}
		else
		{
			if (GetInputSystem()->GetY())
			{
				mIsTutorial = false;
				mTitle1Sprite->SetVisible(true);
				mTitle2Sprite->SetVisible(true);
				mModeSprite->SetVisible(true);
				mBackSprite->SetVisible(false);
				mTutorialSprite->SetVisible(false);

				int index = GetAudioSystem()->LoadSound("se_ok");
				GetAudioSystem()->PlaySoundEx(index, 0);
			}
		}
	}

	BaseScene::ProcessInput();
}

void TitleScene::Update()
{
	if (GetSceneState() == SceneState::EPlay)
	{
		GetParticleManager()->Update(GetRenderer());

		mAlpha += mRate * GetDeltaTime();
		if ((mAlpha <= 0.0f && mRate < 0.0f) || (mAlpha >= 1.0f && mRate > 0.0f))
		{
			mRate *= -1.0f;
		}
		mTitle1Sprite->SetColor(dx::XMFLOAT4{ 1.0f,1.0f,1.0f,mAlpha });
	}

	BaseScene::Update();
}

void TitleScene::GenerateOutput()
{
	BaseScene::GenerateOutput();

	if (GetSceneState() == SceneState::EQuit)
	{
		GetFade()->SetFadeState(Fade::FadeState::EFadeOut);

		if (GetFade()->GetAlpha() >= 1.0f)
		{
			if (GetNextScene() == SceneType::EGame)
			{
				GetAudioSystem()->StopSoundAll();
			}

			Parameter parameter;
			GetSceneManager()->ChangeScene(GetNextScene(), parameter, true);
		}
	}
}
