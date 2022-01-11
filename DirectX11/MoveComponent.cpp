#include "MoveComponent.h"
#include "Actor.h"

namespace dx = DirectX;

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	:
	Component(owner, updateOrder),
	mAngularSpeed(0.0f),
	mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	dx::XMFLOAT3 rotation = mOwner->GetRotation();
	//rotation.x += mAngularSpeed * deltaTime;
	rotation.y += mAngularSpeed * deltaTime;
	rotation.z -= mAngularSpeed * deltaTime;
	mOwner->SetRotation(rotation);
}
