#include "ResultScene.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Parameter.h"
#include "Actor.h"
#include "Renderer.h"
#include "Texture.h"
#include "SpriteComponent.h"
#include "InputSystem.h"
#include "AudioSystem.h"
#include "Fade.h"
#include "Mesh.h"
#include "TranslucenceComponent.h"
#include "CameraComponent.h"
#include "DefineConstant.h"
#include "PlaneActor.h"

namespace dx = DirectX;

ResultScene::ResultScene(SceneManager* sm, const Parameter& parameter)
	:
	BaseScene(sm, parameter),
	mIsGameWin(parameter.GetIsGameWin())
{
	PlaneActor* plane = new PlaneActor(this);

	Renderer* renderer = GetRenderer();
	renderer->ResetLight();

	Actor* actor = new Actor(this);
	actor->SetPosition(dx::XMFLOAT3{ 0.0f,Constant::height,0.0f });
	CameraComponent* cc = new CameraComponent(actor, CameraComponent::CameraType::ENormal);
	cc->SnapToIdeal();

	std::wstring shaderName = mIsGameWin ? L"LaplacianFilter" : L"NegativeFilter";
	actor = new Actor(this);
	Mesh* mesh = renderer->GetMesh("planeScreenshot");
	mesh->ParsePlaneMesh(renderer, "screenshot", shaderName, 2, 12.0f, 6.75f, true);
	TranslucenceComponent* tc = new TranslucenceComponent(actor, mesh);
	actor->SetRotation(dx::XMFLOAT3{ 0.0f,0.0f,0.5f });
	actor->SetPosition(dx::XMFLOAT3{ 1.5f,Constant::height + 12.0f,0.0f });

	std::string textureName = mIsGameWin ? "victory" : "defeat";
	actor = new Actor(this);
	mesh = renderer->GetMesh("planeResult");
	mesh->ParsePlaneMesh(renderer, textureName, L"3DSprite", 2, 5.0f, 4.0f, true);
	tc = new TranslucenceComponent(actor, mesh);
	actor->SetRotation(dx::XMFLOAT3{ 0.0f,0.0f,-0.5f });
	actor->SetPosition(dx::XMFLOAT3{ -4.0f,Constant::height + 12.0f,0.0f });
}

ResultScene::~ResultScene()
{
	GetAudioSystem()->StopSoundAll();
}

void ResultScene::ProcessInput()
{
	if (GetSceneState() == SceneState::EPlay)
	{
		if (mIsGameWin)
		{
			if (GetInputSystem()->GetY())
			{
				SetSceneState(SceneState::EQuit);
				SetNextScene(SceneType::ETitle);

				int index = GetAudioSystem()->LoadSound("se_ok");
				GetAudioSystem()->PlaySoundEx(index, 0);
			}
		}
		else
		{
			if (GetInputSystem()->GetY())
			{
				SetSceneState(SceneState::EQuit);
				SetNextScene(SceneType::ETitle);

				int index = GetAudioSystem()->LoadSound("se_ok");
				GetAudioSystem()->PlaySoundEx(index, 0);
			}
			else if(GetInputSystem()->GetX())
			{
				SetSceneState(SceneState::EQuit);
				SetNextScene(SceneType::EGame);

				int index = GetAudioSystem()->LoadSound("se_ok");
				GetAudioSystem()->PlaySoundEx(index, 0);
			}
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
			GetSceneManager()->ChangeScene(GetNextScene(), parameter, true);
		}
	}
}
