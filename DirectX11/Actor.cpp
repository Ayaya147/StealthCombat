#include "Actor.h"
#include "GameScene.h"
#include "Component.h"

namespace dx = DirectX;

Actor::Actor(BaseScene* scene)
	:
	mWorldTransform(dx::XMMATRIX{}),
	mPosition(dx::XMFLOAT3{}),
	mRotation(dx::XMFLOAT3{}),
	mScale(1.0f),
	mState(ActorState::EActive),
	mScene(scene),
	mRecomputeWorldTransform(true)
{
	mScene->AddActor(this);
}

Actor::~Actor()
{
	mScene->RemoveActor(this);
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (mState == ActorState::EActive)
	{
		ComputeWorldTransform();
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
		ComputeWorldTransform();
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::ComputeWorldTransform()
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;

		mWorldTransform = dx::XMMatrixScaling(mScale, mScale, mScale);
		mWorldTransform *= dx::XMMatrixRotationX(mRotation.x);
		mWorldTransform *= dx::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	}
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
