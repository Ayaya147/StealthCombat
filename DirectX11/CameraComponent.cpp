#include "CameraComponent.h"
#include "Actor.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "Keyboard.h"
#include "GamePad.h"
#include "XMFloatHelper.h"

namespace dx = DirectX;

CameraComponent::CameraComponent(Actor* owner, int updateOrder)
	:
	Component(owner, updateOrder),
	mTargetDist(16.0f),
	mType(1),
	mVelocity(dx::XMFLOAT3{ 0.0f,0.0f,0.0f }),
	mActualPos(dx::XMFLOAT3{ 0.0f,0.0f,0.0f })
{
}

void CameraComponent::Update(float deltaTime)
{
	dx::XMMATRIX view;
	if (mType == 1)
	{
		//dx::XMFLOAT3 cameraPos = dx::XMFLOAT3{
		//	GetOwner()->GetPosition().x,
		//	GetOwner()->GetPosition().y + mTargetDist,
		//	GetOwner()->GetPosition().z
		//};

		//dx::XMFLOAT3 forward = GetOwner()->GetForward();
		//view = dx::XMMatrixLookAtLH(
		//	dx::XMLoadFloat3(&cameraPos),
		//	dx::XMLoadFloat3(&GetOwner()->GetPosition()),
		//	dx::XMLoadFloat3(&forward)
		//);
		float sprintConstant = 64.0f;
		float dampening = 4.0f * sqrtf(sprintConstant);
		dx::XMFLOAT3 idealPos = ComputeCameraPos();
		dx::XMFLOAT3 diff = mActualPos - idealPos;
		dx::XMFLOAT3 accel = -sprintConstant * diff - dampening * mVelocity;
		mVelocity += accel * deltaTime;
		mActualPos += mVelocity * deltaTime;

		dx::XMFLOAT3 forward = GetOwner()->GetForward();
		view = dx::XMMatrixLookAtLH(
			dx::XMLoadFloat3(&mActualPos),
			dx::XMLoadFloat3(&GetOwner()->GetPosition()),
			dx::XMLoadFloat3(&forward)
		);
	}
	else
	{
		dx::XMFLOAT3 forward = GetOwner()->GetForward();
		dx::XMFLOAT3 up = { 0.0f,1.0f,0.0f };
		dx::XMFLOAT3 cameraPos = dx::XMFLOAT3{
			GetOwner()->GetPosition().x - forward.x * mTargetDist * 0.9f,
			GetOwner()->GetPosition().y + 4.0f,
			GetOwner()->GetPosition().z - forward.z * mTargetDist * 0.9f
		};

		view = dx::XMMatrixLookAtLH(
			dx::XMLoadFloat3(&cameraPos),
			dx::XMLoadFloat3(&GetOwner()->GetPosition()),
			dx::XMLoadFloat3(&up)
		);
	}

	GetOwner()->GetScene()->GetRenderer()->SetViewMatrix(view);
}

void CameraComponent::ProcessInput()
{
	GamePad* pad = GetOwner()->GetScene()->GetInputSystem()->GetPad();
	Keyboard* keyboard = GetOwner()->GetScene()->GetInputSystem()->GetKeyboard();

	if (pad->GetIsGamePad())
	{
		if (pad->GetButtonState(XINPUT_GAMEPAD_A) == ButtonState::EPressed)
		{
			if (mType == 1)
			{
				mType = 2;
			}
			else
			{
				mType = 1;
			}
		}
	}
	else
	{
		if (keyboard->GetKeyState('1') == ButtonState::EPressed)
		{
			if (mType == 1)
			{
				mType = 2;
			}
			else
			{
				mType = 1;
			}
		}
	}
}

DirectX::XMFLOAT3 CameraComponent::ComputeCameraPos()
{
	dx::XMFLOAT3 cameraPos = dx::XMFLOAT3{
		GetOwner()->GetPosition().x,
		GetOwner()->GetPosition().y + mTargetDist,
		GetOwner()->GetPosition().z
	};

	return cameraPos;
}

void CameraComponent::SnapToIdeal()
{
	mActualPos = ComputeCameraPos();

	dx::XMFLOAT3 forward = GetOwner()->GetForward();
	dx::XMMATRIX view = dx::XMMatrixLookAtLH(
		dx::XMLoadFloat3(&mActualPos),
		dx::XMLoadFloat3(&GetOwner()->GetPosition()),
		dx::XMLoadFloat3(&forward)
	);

	GetOwner()->GetScene()->GetRenderer()->SetViewMatrix(view);
}
