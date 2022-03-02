#include "CameraComponent.h"
#include "Actor.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "Keyboard.h"
#include "GamePad.h"
#include "XMFloatHelper.h"
#include "Random.h"
#include "DemoScene.h"

namespace dx = DirectX;

CameraComponent::CameraComponent(Actor* owner, int updateOrder)
	:
	Component(owner, updateOrder),
	mState(VibrationState::ENone),
	mTargetDist(20.0f),
	mVelocity(dx::XMFLOAT3{ 0.0f,0.0f,0.0f }),
	mActualPos(dx::XMFLOAT3{ 0.0f,0.0f,0.0f })
{
}

void CameraComponent::ProcessInput()
{
}

void CameraComponent::Update(float deltaTime)
{
	dx::XMMATRIX view;

	if (auto demo = dynamic_cast<DemoScene*>(GetOwner()->GetScene()))
	{
		dx::XMFLOAT3 cameraPos = dx::XMFLOAT3{
			GetOwner()->GetPosition().x,
			GetOwner()->GetPosition().y + mTargetDist,
			GetOwner()->GetPosition().z
		};

		dx::XMFLOAT3 up = GetOwner()->GetForward();
		view = dx::XMMatrixLookAtLH(
			dx::XMLoadFloat3(&cameraPos),
			dx::XMLoadFloat3(&GetOwner()->GetPosition()),
			dx::XMLoadFloat3(&up)
		);
	}
	else
	{
		float sprintConstant = 64.0f;
		float dampening = 5.0f * sqrtf(sprintConstant);
		dx::XMFLOAT3 idealPos = ComputeCameraPos();
		dx::XMFLOAT3 diff = idealPos - mActualPos;
		dx::XMFLOAT3 accel = sprintConstant * diff - dampening * mVelocity;
		mVelocity += accel * deltaTime;
		mActualPos += mVelocity * deltaTime;

		dx::XMFLOAT3 up = GetOwner()->GetForward();
		dx::XMFLOAT3 at = GetOwner()->GetPosition() + GetOwner()->GetForward() * 1.5f;

		float strength = 0.0f;
		switch (mState)
		{
		case CameraComponent::VibrationState::ENone:
			strength = 0.0f;
			break;
		case CameraComponent::VibrationState::ENormal:
			strength = 0.07f;
			break;
		case CameraComponent::VibrationState::EHard:
			strength = 0.4f;
			break;
		}
		at += Random::GetVector() * strength;

		view = dx::XMMatrixLookAtLH(
			dx::XMLoadFloat3(&mActualPos),
			dx::XMLoadFloat3(&at),
			dx::XMLoadFloat3(&up)
		);
	}

	GetOwner()->GetScene()->GetRenderer()->SetViewMatrix(view);
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
