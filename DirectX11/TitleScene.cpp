#include "TitleScene.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "Renderer.h"
#include "Mesh.h"
#include "TransformCBuffer.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "Actor.h"
#include "InputSystem.h"
#include "Keyboard.h"

namespace dx = DirectX;

TitleScene::TitleScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	Renderer* renderer = GetRenderer();

	Actor* sprite = new Actor(this);
	sprite->SetTransformCBuffer(new TransformCBuffer(renderer, sprite));
	Texture* tex = renderer->GetTexture("Assets\\Texture\\test.png");
	SpriteComponent* sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ -700.0f, -400.0f, 0.0f });
	sprite->SetScale(0.5f);

	sprite = new Actor(this);
	sprite->SetTransformCBuffer(new TransformCBuffer(renderer, sprite));
	tex = renderer->GetTexture("Assets\\Texture\\normal1.png");
	sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 0.0f, -400.0f, 0.0f });
	sprite->SetScale(0.2f);
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

	if (GetInputSystem()->GetKeyboard()->KeyIsPressed('C'))
	{
		Parameter parameter;
		mSceneManager->ChangeScene(SceneManager::SceneType::EGame, parameter, true);
	}
}
