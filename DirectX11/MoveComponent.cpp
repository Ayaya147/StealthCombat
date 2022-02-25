#include "MoveComponent.h"
#include "Actor.h"

namespace dx = DirectX;

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	:
	Component(owner, updateOrder),
	mForwardSpeedMax(0.0f),
	mForwardSpeedMin(0.0f),
	mAngularSpeed(0.0f),
	mForwardSpeed(0.0f),
	mAcceleration(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	Actor* owner = GetOwner();

	mForwardSpeed += mAcceleration * deltaTime;
	if (mForwardSpeed > mForwardSpeedMax)
	{
		mForwardSpeed = mForwardSpeedMax;
	}
	else if (mForwardSpeed < mForwardSpeedMin)
	{
		mForwardSpeed = mForwardSpeedMin;
	}

	dx::XMFLOAT3 forward = owner->GetForward();
	dx::XMFLOAT3 pos = owner->GetPosition();
	pos.x += forward.x * mForwardSpeed * deltaTime;
	pos.z += forward.z * mForwardSpeed * deltaTime;
	owner->SetPosition(pos);

	dx::XMFLOAT3 rotation = owner->GetRotation();
	rotation.y += mAngularSpeed * deltaTime;
	rotation.z -= mAngularSpeed * deltaTime;

	if (rotation.z > 0.8f)
	{
		rotation.z = 0.8f;
	}
	else if (rotation.z < -0.8f)
	{
		rotation.z = -0.8f;
	}
	owner->SetRotation(rotation);
}
