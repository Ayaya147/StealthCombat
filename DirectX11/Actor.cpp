#include "Actor.h"
#include "GameScene.h"
#include "Component.h"

namespace dx = DirectX;

Actor::Actor(GameScene* game)
	:
	mWorldTransform(dx::XMMATRIX{}),
	mPosition(dx::XMFLOAT3{}),
	mRotation(dx::XMFLOAT3{}),
	mScale(1.0f),
	mGame(game)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
}

void Actor::Update()
{
	ComputeWorldTransform();
	UpdateComponents();
	UpdateActor();
	ComputeWorldTransform();
}

void Actor::UpdateComponents()
{
}

void Actor::UpdateActor()
{
}

void Actor::ComputeWorldTransform()
{
	mWorldTransform = dx::XMMatrixScaling(mScale, mScale, mScale);
	mWorldTransform *= dx::XMMatrixRotationX(mRotation.x);
	mWorldTransform *= dx::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
}

void Actor::AddComponent(Component* component)
{
	int order = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); iter++)
	{
		if (order < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
