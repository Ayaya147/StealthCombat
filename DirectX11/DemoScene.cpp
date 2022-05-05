#include "DemoScene.h"
#include <algorithm>
#include "SceneManager.h"
#include "Parameter.h"
#include "PlayerActor.h"
#include "CloudActor.h"
#include "PlaneActor.h"
#include "EmissionActor.h"
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
#include "AudioSystem.h"
#include "ParticleManager.h"

namespace dx = DirectX;

DemoScene::DemoScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter),
	mPlane(new PlaneActor(this)),
	mCloud(new CloudActor(this)),
	mExplosion(new ExplosionActor(this)),
	mSmoke(new SmokeActor(this)),
	mEmission(new EmissionActor(this)),
	mParticleManager(new ParticleManager(this, GetRenderer()))
{
	ShowCursor(TRUE);

	mParticleManager->CreateParticleSystem(GetRenderer());
	
	mCloud->SetScale(dx::XMFLOAT3{ 10.0f,5.0f,10.0f });
	mCloud->SetPosition(dx::XMFLOAT3{ -7.0f,Constant::height,0.0f });

	mExplosion->SetScale(16.0f);
	mExplosion->SetPosition(dx::XMFLOAT3{ 6.0f,Constant::height,0.0f });
	
	mSmoke->SetScale(12.0f);
	mSmoke->SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,5.0f });

	mEmission->SetScale(7.0f);
	mEmission->SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,-5.0f });
	
	PlayerActor* player = new PlayerActor(this);

	Actor* sprite = new Actor(this);
	Texture* tex = GetRenderer()->GetTexture("back");
	SpriteComponent* sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 750.0f, 470.0f, 0.0f });
	sprite->SetScale(0.4f);

	GetRenderer()->ResetLight();
}

DemoScene::~DemoScene()
{
#ifdef NDEBUG
	ShowCursor(FALSE);
#endif

	delete mParticleManager;
}

void DemoScene::ProcessInput()
{
	if (GetSceneState() == SceneState::EPlay && GetInputSystem()->GetY())
	{
		SetSceneState(SceneState::EQuit);

		int index = GetAudioSystem()->LoadSound("se_ok");
		GetAudioSystem()->PlaySoundEx(index, 0);
	}

	BaseScene::ProcessInput();
}

void DemoScene::Update()
{
	switch (GetSceneState())
	{
	case SceneState::EPlay:
		mParticleManager->Update(GetRenderer());
		break;
	}

	BaseScene::Update();
}

void DemoScene::GenerateOutput()
{
	BaseScene::GenerateOutput();

	if (GetSceneState() == SceneState::EQuit)
	{
		GetFade()->SetFadeState(Fade::FadeState::EFadeOut);
		if (GetFade()->GetAlpha() >= 1.0f)
		{
			Parameter parameter;
			parameter.SetIsFromDemo(true);
			GetSceneManager()->ChangeScene(SceneManager::SceneType::ETitle, parameter, true);
		}
	}
}
