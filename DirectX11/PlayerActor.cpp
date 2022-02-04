#include "PlayerActor.h"
#include "BaseScene.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Mesh.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "TransformCBuffer.h"
#include "InputSystem.h"
#include "Keyboard.h"
#include "GamePad.h"

PlayerActor::PlayerActor(BaseScene* scene)
	:
	Actor(scene)
{
	Renderer* renderer = GetScene()->GetRenderer();
	SetScale(0.1f);

	SetTransformCBuffer(new TransformCBuffer(renderer, this));

	Mesh* mesh = renderer->GetMesh("player");
	mesh->ParseMesh(renderer, "player", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
}

void PlayerActor::UpdateActor(float deltaTime)
{
	DirectX::XMFLOAT3 rotation = GetRotation();

	if (rotation.z > 0.8f)
	{
		rotation.z = 0.8f;
	}
	else if (rotation.z < -0.8f)
	{
		rotation.z = -0.8f;
	}
	SetRotation(rotation);
}

void PlayerActor::ActorInput()
{
	Keyboard* keyboard = GetScene()->GetInputSystem()->GetKeyboard();
	GamePad* pad = GetScene()->GetInputSystem()->GetPad();

	DirectX::XMFLOAT3 rotation = GetRotation();

	//if (pad->GetThumbLeftX() > 0)
	//{
	//	pad->SetRightVibration(100);
	//	mMoveComponent->SetAngularSpeed(1.2f);
	//}
	//else if (pad->GetThumbLeftX() < 0)
	//{
	//	pad->SetLeftVibration(100);
	//	mMoveComponent->SetAngularSpeed(-1.2f);
	//}
	//else
	//{
	//	pad->StopVibration();
	//	mMoveComponent->SetAngularSpeed(0.0f);
	//	rotation.z = GetRotation().z * 0.95f;
	//	SetRotation(rotation);
	//}

	//if (pad->GetRightTrigger())
	//{
	//	mMoveComponent->SetForwardSpeed(10.0f);
	//}
	//else
	//{
	//	float spd = mMoveComponent->GetForwardSpeed();
	//	mMoveComponent->SetForwardSpeed(spd * 0.95f);
	//}

	if (keyboard->KeyIsHeld('D') || keyboard->KeyIsPressed('D'))
	{
		mMoveComponent->SetAngularSpeed(1.2f);
	}
	else if (keyboard->KeyIsHeld('A') || keyboard->KeyIsPressed('A'))
	{
		mMoveComponent->SetAngularSpeed(-1.2f);
	}
	else
	{
		float spd = mMoveComponent->GetAngularSpeed();
		mMoveComponent->SetAngularSpeed(spd * 0.9f);
		rotation.z = GetRotation().z * 0.95f;
		SetRotation(rotation);
	}

	if (keyboard->KeyIsHeld('W') || keyboard->KeyIsPressed('W'))
	{
		mMoveComponent->SetForwardSpeed(10.0f);
	}
	else
	{
		float spd = mMoveComponent->GetForwardSpeed();
		mMoveComponent->SetForwardSpeed(spd * 0.95f);
	}
}
