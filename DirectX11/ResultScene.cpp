#include "ResultScene.h"
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

ResultScene::ResultScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	Renderer* renderer = GetRenderer();

	Actor* sprite = new Actor(this);
	sprite->SetTransformCBuffer(new TransformCBuffer(renderer, sprite));
	Texture* tex = renderer->GetTexture("Assets\\Texture\\guide.png");
	SpriteComponent* sc = new SpriteComponent(sprite, tex);

	sprite = new Actor(this);
	sprite->SetTransformCBuffer(new TransformCBuffer(renderer, sprite));
	tex = renderer->GetTexture("Assets\\Texture\\minimap.png");
	sc = new SpriteComponent(sprite, tex);
	sprite->SetPosition(dx::XMFLOAT3{ 0.0f, -400.0f, 0.0f });
}

ResultScene::~ResultScene()
{
}

void ResultScene::ProcessInput()
{
	BaseScene::ProcessInput();
}

void ResultScene::Update()
{
	BaseScene::Update();
}

void ResultScene::GenerateOutput()
{
	BaseScene::GenerateOutput();

	if (GetInputSystem()->GetKeyboard()->KeyIsPressed(VK_RETURN))
	{
		Parameter parameter;
		mSceneManager->ChangeScene(SceneManager::SceneType::ETitle, parameter, true);
	}
}
