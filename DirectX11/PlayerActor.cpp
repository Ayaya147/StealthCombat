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
	SetScale(0.1f);
	Renderer* renderer = GetScene()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("player");
	mesh->ParseMesh(renderer, "player", L"Phong");
	MeshComponent* mc = new MeshComponent(this, mesh);

	mMoveComponent = new MoveComponent(this);
	mMoveComponent->SetForwardSpeedMax(20.0f);
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

	if (pad->GetIsGamePad())
	{
		if (pad->GetThumbLeftX() > 0)
		{
			pad->SetRightVibration(100);
			mMoveComponent->SetAngularSpeed(1.2f);
		}
		else if (pad->GetThumbLeftX() < 0)
		{
			pad->SetLeftVibration(100);
			mMoveComponent->SetAngularSpeed(-1.2f);
		}
		else
		{
			pad->StopVibration();
			float spd = mMoveComponent->GetAngularSpeed();
			mMoveComponent->SetAngularSpeed(spd * 0.9f);
			rotation.z = GetRotation().z * 0.95f;
			SetRotation(rotation);
		}

		//if (pad->GetButtonValue(XINPUT_GAMEPAD_X))
		if (pad->GetRightTrigger())
		{
			mMoveComponent->SetAcceleration(5.0f);
		}
		else
		{
			mMoveComponent->SetAcceleration(-5.0f);
		}
	}
	else
	{
		if (keyboard->GetKeyValue('D'))
		{
			mMoveComponent->SetAngularSpeed(1.2f);
		}
		else if (keyboard->GetKeyValue('A'))
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

		if (keyboard->GetKeyValue('W'))
		{
			mMoveComponent->SetAcceleration(5.0f);
		}
		else if (keyboard->GetKeyValue('S'))
		{
			mMoveComponent->SetAcceleration(-7.0f);
		}
		else
		{
			mMoveComponent->SetAcceleration(-2.0f);
		}
	}
}

float PlayerActor::GetForwardSpeed() const
{
	return mMoveComponent->GetForwardSpeed();
}
