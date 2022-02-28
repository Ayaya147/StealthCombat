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
	BaseScene(sm, parameter)
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
	sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 25.0f, 300.0f, 0.0f });
	sprite->SetScale(0.5f);

	sprite = new Actor(this);
	tex = renderer->GetTexture("title");
	sc = new SpriteComponent(sprite, tex);
	sprite->SetScale(1.0f);
}

TitleScene::~TitleScene()
{
}

void TitleScene::ProcessInput()
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

	}
	else if (GetInputSystem()->GetA())
	{
		PostQuitMessage(0);
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

		switch (gNextScene)
		{
		case SceneManager::SceneType::EDemo:
			if (GetFade()->GetAlpha() >= 1.0f)
			{
				Parameter parameter;
				GetSceneManager()->ChangeScene(SceneManager::SceneType::EDemo, parameter, true);
			}
			break;

		case SceneManager::SceneType::EGame:
			if (GetFade()->GetAlpha() >= 1.0f)
			{
				Parameter parameter;
				GetSceneManager()->ChangeScene(SceneManager::SceneType::EGame, parameter, true);
			}
			break;
		}
	}
}
