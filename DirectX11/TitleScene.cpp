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

namespace dx = DirectX;

TitleScene::TitleScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	Renderer* renderer = GetRenderer();

	Actor* sprite = new Actor(this);
	Texture* tex = renderer->GetTexture("0");
	SpriteComponent* sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);

	sprite = new Actor(this);
	tex = renderer->GetTexture("1");
	sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);
	sprite->SetPosition(dx::XMFLOAT3{ 0.0f, -400.0f, 0.0f });
}

TitleScene::~TitleScene()
{
}

void TitleScene::ProcessInput()
{
	BaseScene::ProcessInput();
}

void TitleScene::Update()
{
	BaseScene::Update();
}

void TitleScene::GenerateOutput()
{
	BaseScene::GenerateOutput();

	if (GetInputSystem()->GetSceneChangeEnter())
	{
		GetInputSystem()->GetPad()->StopVibration();
		Parameter parameter;
		GetSceneManager()->ChangeScene(SceneManager::SceneType::EGame, parameter, true);
	}
	else if (GetInputSystem()->GetSceneChangeSpace())
	{
		GetInputSystem()->GetPad()->StopVibration();
		Parameter parameter;
		GetSceneManager()->ChangeScene(SceneManager::SceneType::EDemo, parameter, true);
	}
}
