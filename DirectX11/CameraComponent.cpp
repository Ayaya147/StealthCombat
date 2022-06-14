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
#include "GameScene.h"
#include "ResultScene.h"

namespace dx = DirectX;

CameraComponent::CameraComponent(Actor* owner, CameraType type, int updateOrder)
	:
	Component(owner, updateOrder),
	mType(type),
	mState(VibrationState::ENone),
	mTargetDist(20.0f),
	mVelocity(dx::XMFLOAT3{ 0.0f,0.0f,0.0f }),
	mActualPos(dx::XMFLOAT3{ 0.0f,0.0f,0.0f }),
	mUp(dx::XMFLOAT3{ 0.0f,0.0f,0.0f }),
	mAt(GetOwner()->GetPosition())
{
}

void CameraComponent::ProcessInput()
{
	InputSystem* input = GetOwner()->GetScene()->GetInputSystem();

	if (auto game = dynamic_cast<GameScene*>(GetOwner()->GetScene()))
	{
		if (input->GetCameraDown())
		{
			mUp = GetOwner()->GetForward();
			mUp.x *= -1.0f;
			mUp.z *= -1.0f;
		}
		else
		{
			mUp = GetOwner()->GetForward();
		}
	}
	else if (auto result = dynamic_cast<ResultScene*>(GetOwner()->GetScene()))
	{
		GamePad* pad = input->GetPad();
		float rate = 0.0f;
		float rangeX = 3.0f;
		float rangeY = 3.0f;
		float speedBack = 8.0f;
		float deltaTime = result->GetDeltaTime();
		float speed = 8.0f;
		float speedY = 0.0f;
		float speedX = 0.0f;
		if (pad->GetThumbRightY() != 0)
		{
			rate = abs((float)pad->GetThumbRightX() / (float)pad->GetThumbRightY());
			speedY = sqrtf((speed * speed) / (rate * rate + 1));
			speedX = rate * sqrtf((speed * speed) / (rate * rate + 1));
		}
		else
		{
			speedX = speed;
			speedY = 0.0f;
		}

		if (input->GetCameraLeft())
		{
			mAt.x -= speedX * deltaTime;
			if (mAt.x <= -rangeX)
			{
				mAt.x = -rangeX;
			}
		}
		else if (input->GetCameraRight())
		{
			mAt.x += speedX * deltaTime;
			if (mAt.x >= rangeX)
			{
				mAt.x = rangeX;
			}
		}

		if (input->GetCameraUp())
		{
			mAt.z += speedY * deltaTime;
			if (mAt.z >= rangeY)
			{
				mAt.z = rangeY;
			}
		}
		else if (input->GetCameraDown())
		{
			mAt.z -= speedY * deltaTime;
			if (mAt.z <= -rangeY)
			{
				mAt.z = -rangeY;
			}
		}

		if (!input->GetCameraLeft() &&
			!input->GetCameraRight() &&
			!input->GetCameraUp() &&
			!input->GetCameraDown())
		{
			if (mAt.x > 0)
			{
				mAt.x -= speedBack * deltaTime;
				if (mAt.x <= 0)
				{
					mAt.x = 0.0f;
				}
			}
			else if (mAt.x < 0)
			{
				mAt.x += speedBack * deltaTime;
				if (mAt.x >= 0)
				{
					mAt.x = 0.0f;
				}
			}

			if (mAt.z > 0)
			{
				mAt.z -= speedBack * deltaTime;
				if (mAt.z <= 0)
				{
					mAt.z = 0.0f;
				}
			}
			else if (mAt.z < 0)
			{
				mAt.z += speedBack * deltaTime;
				if (mAt.z >= 0)
				{
					mAt.z = 0.0f;
				}
			}
		}

		mUp = GetOwner()->GetForward();
	}
	else
	{
		mUp = GetOwner()->GetForward();
		mAt = GetOwner()->GetPosition();
	}
}

void CameraComponent::Update(float deltaTime)
{
	dx::XMMATRIX view = {};
	dx::XMFLOAT3 at = {};

	switch (mType)
	{
	case CameraComponent::CameraType::ENormal:
		dx::XMFLOAT3 cameraPos = dx::XMFLOAT3{
			GetOwner()->GetPosition().x,
			GetOwner()->GetPosition().y + mTargetDist,
			GetOwner()->GetPosition().z
		};

		view = dx::XMMatrixLookAtLH(
			dx::XMLoadFloat3(&cameraPos),
			dx::XMLoadFloat3(&mAt),
			dx::XMLoadFloat3(&mUp)
		);

		break;

	case CameraComponent::CameraType::ESpring:
	{
		float sprintConstant = 64.0f;
		float dampening = 5.0f * sqrtf(sprintConstant);
		dx::XMFLOAT3 idealPos = ComputeCameraPos();
		dx::XMFLOAT3 diff = idealPos - mActualPos;
		dx::XMFLOAT3 accel = sprintConstant * diff - dampening * mVelocity;
		mVelocity += accel * deltaTime;
		mActualPos += mVelocity * deltaTime;

		at = GetOwner()->GetPosition() + GetOwner()->GetForward() * 1.5f;

		float strength = 0.0f;
		switch (mState)
		{
		case CameraComponent::VibrationState::ENone:
			strength = 0.0f;
			break;
		case CameraComponent::VibrationState::ELight:
			strength = 0.05f;
			break;
		case CameraComponent::VibrationState::ENormal:
			strength = 0.1f;
			break;
		case CameraComponent::VibrationState::EHard:
			strength = 0.5f;
			break;
		}
		at += Random::GetVector() * strength;

		view = dx::XMMatrixLookAtLH(
			dx::XMLoadFloat3(&mActualPos),
			dx::XMLoadFloat3(&at),
			dx::XMLoadFloat3(&mUp)
		);
	}
		break;
	}

	dx::XMFLOAT4X4 v;
	dx::XMStoreFloat4x4(&v, view);
	GetOwner()->GetScene()->GetRenderer()->SetViewMatrix(v);
	GetOwner()->GetScene()->GetRenderer()->SetViewAtPosition(at);
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

	dx::XMFLOAT4X4 v;
	dx::XMStoreFloat4x4(&v, view);
	GetOwner()->GetScene()->GetRenderer()->SetViewMatrix(v);
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
