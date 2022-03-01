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

namespace dx = DirectX;

static SceneManager::SceneType gNextScene = SceneManager::SceneType::EGame;

TitleScene::TitleScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter),
	mIsTutorial(false)
{
	Renderer* renderer = GetRenderer();

	PlaneActor* plane = new PlaneActor(this);
	CloudActor* cloud = new CloudActor(this);
	cloud->SetScale(dx::XMFLOAT3{ 20.0f,1.0f,20.0f });
	cloud->SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,1.5f });

	Actor* actor = new Actor(this);
	actor->SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,0.0f });
	CameraComponent* cc = new CameraComponent(actor);
	cc->SnapToIdeal();

	Actor* sprite = new Actor(this);
	Texture* tex = renderer->GetTexture("fade");
	SpriteComponent* sc = new SpriteComponent(sprite, tex);
	sprite->SetScale(5.0f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("mode");
	mModeSprite = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 25.0f, 300.0f, 0.0f });
	sprite->SetScale(0.5f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("title");
	mTitleSprite = new SpriteComponent(sprite, tex);
	sprite->SetScale(1.0f);

	sprite = new Actor(this);
	tex = GetRenderer()->GetTexture("back");
	mBackSprite = new SpriteComponent(sprite, tex);
	mBackSprite->SetVisible(false);
	sprite->SetPosition(dx::XMFLOAT3{ 750.0f, 470.0f, 0.0f });
	sprite->SetScale(0.4f);

	sprite = new Actor(this);
	tex = GetRenderer()->GetTexture("tutorial");
	mTutorialSprite = new SpriteComponent(sprite, tex);
	mTutorialSprite->SetVisible(false);
	sprite->SetScale(1.0f);
}

TitleScene::~TitleScene()
{
}

void TitleScene::ProcessInput()
{
	if (!mIsTutorial)
	{
		if (GetInputSystem()->GetX())
		{
			SetSceneState(SceneState::EQuit);
			gNextScene = SceneManager::SceneType::EGame;
		}
		else if (GetInputSystem()->GetY())
		{
			SetSceneState(SceneState::EQuit);
			gNextScene = SceneManager::SceneType::EDemo;
		}
		else if (GetInputSystem()->GetB())
		{
			mIsTutorial = true;
			mTitleSprite->SetVisible(false);
			mModeSprite->SetVisible(false);
			mBackSprite->SetVisible(true);
			mTutorialSprite->SetVisible(true);
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
			mTitleSprite->SetVisible(true);
			mModeSprite->SetVisible(true);
			mBackSprite->SetVisible(false);
			mTutorialSprite->SetVisible(false);
		}
	}

	BaseScene::ProcessInput();
}

void TitleScene::Update()
{
	BaseScene::Update();
}

void TitleScene::GenerateOutput()
{
	BaseScene::GenerateOutput();

	if (GetSceneState() == SceneState::EQuit)
	{
		GetInputSystem()->GetPad()->StopVibration();
		GetFade()->SetFadeState(Fade::FadeState::EFadeOut);

		if (GetFade()->GetAlpha() >= 1.0f)
		{
			Parameter parameter;
			GetSceneManager()->ChangeScene(gNextScene, parameter, true);
		}
	}
}
