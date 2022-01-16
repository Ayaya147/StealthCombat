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

PlayerActor::PlayerActor(BaseScene* scene, const std::string& fileName)
	:
	Actor(scene)
{
	Renderer* renderer = GetScene()->GetRenderer();

	SetTransformCBuffer(new TransformCBuffer(renderer, this));
	//SetScale(1.0f);

	Mesh* mesh = renderer->GetMesh(fileName, L"Phong", 1);
	//Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh(fileName, L"Raymarching", 1);
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
	//move->SetAngularSpeed(0.2f);
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

	//if (input->IsButtonTriggered(XINPUT_GAMEPAD_DPAD_RIGHT))
	//{
	//	//input->SetVibration(255);
	//	mMoveComponent->SetAngularSpeed(0.2f);
	//}
	//if (input->IsButtonTriggered(XINPUT_GAMEPAD_DPAD_LEFT))
	//{
	//	//input->SetVibration(255);
	//	mMoveComponent->SetAngularSpeed(-0.2f);
	//}
	DirectX::XMFLOAT3 rotation = GetRotation();

	if (keyboard->KeyIsPressed('D'))
	{
		mMoveComponent->SetAngularSpeed(0.6f);
	}
	else if (keyboard->KeyIsPressed('A'))
	{
		mMoveComponent->SetAngularSpeed(-0.6f);
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
