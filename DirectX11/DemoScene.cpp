#include "DemoScene.h"
#include <algorithm>
#include "SceneManager.h"
#include "Parameter.h"
#include "PlayerActor.h"
#include "CloudActor.h"
#include "PlaneActor.h"
#include "ExplosionActor.h"
#include "SmokeActor.h"
#include "InputSystem.h"
#include "GamePad.h"
#include "Keyboard.h"
#include "DefineConstant.h"
#include "Fade.h"
#include "Texture.h"
#include "Renderer.h"
#include "SpriteComponent.h"

namespace dx = DirectX;

DemoScene::DemoScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	ShowCursor(TRUE);

	mPlane = new PlaneActor(this);
	mCloud = new CloudActor(this);
	mCloud->SetScale(dx::XMFLOAT3{ 10.0f,5.0f,10.0f });
	mCloud->SetPosition(dx::XMFLOAT3{ -7.0f,Constant::height,0.0f });

	mExplosion = new ExplosionActor(this);
	mExplosion->SetPosition(dx::XMFLOAT3{ 6.0f,Constant::height,0.0f });
	mSmoke = new SmokeActor(this);
	mSmoke->SetScale(10.0f);
	mSmoke->SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,-5.0f });
	PlayerActor* player = new PlayerActor(this);

	Actor* sprite = new Actor(this);
	Texture* tex = GetRenderer()->GetTexture("back");
	SpriteComponent* sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 700.0f, 420.0f, 0.0f });
	sprite->SetScale(0.5f);
}

DemoScene::~DemoScene()
{
#ifdef NDEBUG
	ShowCursor(FALSE);
#endif
}

void DemoScene::ProcessInput()
{
	if (GetInputSystem()->GetY())
	{
		SetSceneState(SceneState::EQuit);
	}

	BaseScene::ProcessInput();
}

void DemoScene::Update()
{
	BaseScene::Update();
}

void DemoScene::GenerateOutput()
{
	BaseScene::GenerateOutput();

	if (GetSceneState() == SceneState::EQuit)
	{
		GetInputSystem()->GetPad()->StopVibration();
		GetFade()->SetFadeState(Fade::FadeState::EFadeOut);
		if (GetFade()->GetAlpha() >= 1.0f)
		{
			Parameter parameter;
			GetSceneManager()->ChangeScene(SceneManager::SceneType::ETitle, parameter, true);
		}
	}
}
