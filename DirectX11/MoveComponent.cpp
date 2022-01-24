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
