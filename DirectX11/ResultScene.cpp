#include "ResultScene.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "Actor.h"
#include "Texture.h"
#include "SpriteComponent.h"
#include "InputSystem.h"
#include "AudioSystem.h"
#include "Fade.h"

namespace dx = DirectX;

ResultScene::ResultScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	Renderer* renderer = GetRenderer();

	Actor* sprite = new Actor(this);
	Texture* tex = renderer->GetTexture("title_02");
	SpriteComponent* title2Sprite = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 200.0f, 0.0f, 0.0f });
	sprite->SetScale(1.0f);
}

ResultScene::~ResultScene()
{
	GetAudioSystem()->StopSoundAll();
}

void ResultScene::ProcessInput()
{
	if (GetSceneState() == SceneState::EPlay)
	{
		if (GetInputSystem()->GetX())
		{
			SetSceneState(SceneState::EQuit);

			int index = GetAudioSystem()->LoadSound("se_ok");
			GetAudioSystem()->PlaySoundEx(index, 0);
		}
	}

	BaseScene::ProcessInput();
}

void ResultScene::Update()
{
	BaseScene::Update();
}

void ResultScene::GenerateOutput()
{
	BaseScene::GenerateOutput();

	if (GetSceneState() == SceneState::EQuit)
	{
		GetFade()->SetFadeState(Fade::FadeState::EFadeOut);

		if (GetFade()->GetAlpha() >= 1.0f)
		{
			Parameter parameter;
			GetSceneManager()->ChangeScene(SceneManager::SceneType::ETitle, parameter, true);
		}
	}
}
