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

namespace dx = DirectX;

DemoScene::DemoScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter)
{
	ShowCursor(TRUE);
	mPlane = new PlaneActor(this);
	mCloud = new CloudActor(this);
	mExplosion = new ExplosionActor(this);
	mExplosion->SetPosition(dx::XMFLOAT3{ 6.0f,Constant::height,0.0f });
	mSmoke = new SmokeActor(this);
	mSmoke->SetScale(10.0f);
	mSmoke->SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,-5.0f });
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

	if (GetSceneState() == SceneState::EQuit || GetInputSystem()->GetSceneChangeEnter())
	{
		GetInputSystem()->GetPad()->StopVibration();
		Parameter parameter;
		GetSceneManager()->ChangeScene(SceneManager::SceneType::EResult, parameter, true);
	}
}
