#include "MoveComponent.h"
#include "Actor.h"

namespace dx = DirectX;

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	:
	Component(owner, updateOrder),
	mForwardSpeedMax(0.0f),
	mAngularSpeed(0.0f),
	mForwardSpeed(0.0f),
	mAcceleration(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	float forwardSpeedMin = 800.0f / 160.0f;
	mForwardSpeed += mAcceleration * deltaTime;
	if (mForwardSpeed > mForwardSpeedMax)
	{
		mForwardSpeed = mForwardSpeedMax;
	}
	else if (mForwardSpeed < forwardSpeedMin)
	{
		mForwardSpeed = forwardSpeedMin;
	}

	dx::XMFLOAT3 rotation = GetOwner()->GetRotation();
	rotation.y += mAngularSpeed * deltaTime;
	rotation.z -= mAngularSpeed * deltaTime;
	GetOwner()->SetRotation(rotation);

	dx::XMFLOAT3 forward = GetOwner()->GetForward();
	dx::XMFLOAT3 pos = GetOwner()->GetPosition();
	pos.x += forward.x * mForwardSpeed * deltaTime;
	pos.z += forward.z * mForwardSpeed * deltaTime;
	GetOwner()->SetPosition(pos);
}
