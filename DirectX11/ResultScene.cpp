#include "ResultScene.h"
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

namespace dx = DirectX;

ResultScene::ResultScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	Renderer* renderer = GetRenderer();

	Actor* sprite = new Actor(this);
	Texture* tex = renderer->GetTexture("guide_keyboard");
	SpriteComponent* sc = new SpriteComponent(sprite, tex);

	sprite = new Actor(this);
	tex = renderer->GetTexture("minimap");
	sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 0.0f, -400.0f, 0.0f });
}

ResultScene::~ResultScene()
{
}

void ResultScene::ProcessInput()
{
	if (GetInputSystem()->GetSceneChangeEnter())
	{
		SetSceneState(SceneState::EQuit);
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
		GetInputSystem()->GetPad()->StopVibration();
		GetFade()->SetFadeState(Fade::FadeState::EFadeOut);
		if (GetFade()->GetAlpha() >= 1.0f)
		{
			Parameter parameter;
			GetSceneManager()->ChangeScene(SceneManager::SceneType::ETitle, parameter, true);
		}
	}
}
