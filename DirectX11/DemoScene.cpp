#include "DemoScene.h"
#include <algorithm>
#include "SceneManager.h"
#include "Parameter.h"
#include "Renderer.h"
#include "PlayerActor.h"
#include "EnemyActor.h"
#include "MissileActor.h"
#include "CloudActor.h"
#include "PlaneActor.h"
#include "NumberActor.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "TransformCBuffer.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "InputSystem.h"
#include "GamePad.h"
#include "Keyboard.h"
#include "Random.h"
#include "PhysWorld.h"

namespace dx = DirectX;

DemoScene::DemoScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	ShowCursor(TRUE);
	mPlane = new PlaneActor(this);
	mCloud = new CloudActor(this);
	PlayerActor* player = new PlayerActor(this);
}

DemoScene::~DemoScene()
{
#ifdef NDEBUG
	ShowCursor(FALSE);
#endif
}

void DemoScene::ProcessInput()
{
	BaseScene::ProcessInput();
}

void DemoScene::Update()
{
	if (GetSceneState() == SceneState::EPlay)
	{
	}

	BaseScene::Update();
}

void DemoScene::GenerateOutput()
{
	BaseScene::GenerateOutput();

	GamePad* pad = GetInputSystem()->GetPad();
	Keyboard* keyboard = GetInputSystem()->GetKeyboard();
	if (pad->GetIsGamePad())
	{
		if (GetSceneState() == SceneState::EQuit ||
			pad->GetButtonState(XINPUT_GAMEPAD_RIGHT_THUMB) == ButtonState::EPressed)
		{
			pad->StopVibration();
			Parameter parameter;
			GetSceneManager()->ChangeScene(SceneManager::SceneType::EResult, parameter, true);
		}
	}
	else
	{
		if (GetSceneState() == SceneState::EQuit ||
			keyboard->GetKeyState(VK_RETURN) == ButtonState::EPressed)
		{
			Parameter parameter;
			GetSceneManager()->ChangeScene(SceneManager::SceneType::EResult, parameter, true);
		}
	}
}
