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
#include "Window.h"

PlayerActor::PlayerActor(BaseScene* scene)
	:
	Actor(scene)
{
	Renderer* renderer = GetScene()->GetRenderer();

	SetTransformCBuffer(new TransformCBuffer(renderer, this));

	Mesh* mesh = renderer->GetMesh("player", L"Phong");
	mesh->ParseMesh(renderer, "player", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
}

void PlayerActor::UpdateActor(float deltaTime)
{
	DirectX::XMFLOAT3 rotation = GetRotation();

	if (rotation.z > 0.79f)
	{
		rotation.z = 0.79f;
	}
	else if (rotation.z < -0.79f)
	{
		rotation.z = -0.79f;
	}
	SetRotation(rotation);
}

void PlayerActor::ActorInput()
{
	InputSystem* input = GetScene()->GetInputSystem();
	Keyboard* keyboard = GetScene()->GetWindow()->keyboard;

	DirectX::XMFLOAT3 rotation = GetRotation();

	if (input->IsButtonTriggered(XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		//input->SetVibration(255);
		mMoveComponent->SetAngularSpeed(0.2f);
	}
	if (input->IsButtonTriggered(XINPUT_GAMEPAD_DPAD_LEFT))
	{
		//input->SetVibration(255);
		mMoveComponent->SetAngularSpeed(-0.2f);
	}

	if (keyboard->KeyIsPressed('D'))
	{
		mMoveComponent->SetAngularSpeed(1.2f);
	}
	else if (keyboard->KeyIsPressed('A'))
	{
		mMoveComponent->SetAngularSpeed(-1.2f);
	}
	else
	{
		mMoveComponent->SetAngularSpeed(0.0f);
		rotation.z = GetRotation().z * 0.95f;
		SetRotation(rotation);
	}

	if (keyboard->KeyIsPressed('W'))
	{
		mMoveComponent->SetForwardSpeed(20.0f);
	}
	else
	{
		float spd = mMoveComponent->GetForwardSpeed();
		mMoveComponent->SetForwardSpeed(spd * 0.95f);
	}
}
