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
	SetTransformCBuffer(new TransformCBuffer(scene->GetSceneManager()->GetRenderer(), this));
	//SetScale(1.0f);

	Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh(fileName, L"Phong", 1);
	//Mesh* mesh = GetScene()->GetSceneManager()->GetRenderer()->GetMesh(fileName, L"Raymarching", 1);
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
	//move->SetAngularSpeed(0.2f);
}

void PlayerActor::UpdateActor(float deltaTime)
{
	//DirectX::XMFLOAT3 rotation = GetRotation();
	//SetRotation(DirectX::XMFLOAT3{ rotation.x + 3.0f * deltaTime,rotation.y,rotation.z });
}

void PlayerActor::ActorInput()
{
	InputSystem* input = GetScene()->GetSceneManager()->GetInputSystem();
	Keyboard* keyboard = GetScene()->GetSceneManager()->GetWindow()->keyboard;

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
		mMoveComponent->SetAngularSpeed(0.2f);
	}
	if (keyboard->KeyIsPressed('A'))
	{
		mMoveComponent->SetAngularSpeed(-0.2f);
	}
}
