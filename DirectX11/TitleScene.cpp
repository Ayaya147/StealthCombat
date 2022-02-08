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
#include "GamePad.h"

namespace dx = DirectX;

TitleScene::TitleScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	Renderer* renderer = GetRenderer();

	Actor* sprite = new Actor(this);
	Texture* tex = renderer->GetTexture("Assets\\Texture\\0.png");
	SpriteComponent* sc = new SpriteComponent(sprite);
	sc->SetTexture(tex);

	sprite = new Actor(this);
	tex = renderer->GetTexture("Assets\\Texture\\1.png");
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

	GamePad* pad = GetInputSystem()->GetPad();
	Keyboard* keyboard = GetInputSystem()->GetKeyboard();

	if (!pad->GetIsGamePad() && keyboard->GetKeyState(VK_RETURN) == ButtonState::EPressed)
	{
		Parameter parameter;
		mSceneManager->ChangeScene(SceneManager::SceneType::EGame, parameter, true);
	}
	else if (pad->GetIsGamePad() && pad->GetButtonState(XINPUT_GAMEPAD_RIGHT_THUMB) == ButtonState::EPressed)
	{
		Parameter parameter;
		mSceneManager->ChangeScene(SceneManager::SceneType::EGame, parameter, true);
	}
}
