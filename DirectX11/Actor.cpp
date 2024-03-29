#include "Actor.h"
#include "GameScene.h"
#include "Component.h"
#include "TransformCBuffer.h"
#include "Renderer.h"
#include "SceneManager.h"

namespace dx = DirectX;

Actor::Actor(BaseScene* scene)
	:
	mWorldTransform(dx::XMFLOAT4X4{}),
	mPosition(dx::XMFLOAT3{}),
	mRotation(dx::XMFLOAT3{}),
	mScale(dx::XMFLOAT3{ 1.0f,1.0f,1.0f }),
	mState(ActorState::EActive),
	mScene(scene),
	mRecomputeWorldTransform(true)
{
	mScene->AddActor(this);
	mTransformCBuffer = new TransformCBuffer(GetScene()->GetRenderer(), this);
}

Actor::~Actor()
{
	mScene->RemoveActor(this);
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}

	delete mTransformCBuffer;
	mTransformCBuffer = nullptr;
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

void Actor::ProcessInput()
{
	if (mState == ActorState::EActive)
	{
		for (auto comp : mComponents)
		{
			comp->ProcessInput();
		}
		ActorInput();
	}
}

void Actor::ActorInput()
{
}

void Actor::Bind(Renderer* renderer)
{
	if (mTransformCBuffer)
	{
		mTransformCBuffer->Bind(renderer);
	}
}

void Actor::Bind(Renderer* renderer, float width, float height)
{
	if (mTransformCBuffer)
	{
		mTransformCBuffer->Bind(renderer, width, height);
	}
}

void Actor::ComputeWorldTransform()
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;

		dx::XMMATRIX worldTransform = dx::XMMatrixScaling(mScale.x, mScale.y, mScale.z);
		worldTransform *= dx::XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
		worldTransform *= dx::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

		dx::XMStoreFloat4x4(&mWorldTransform, worldTransform);

		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

void Actor::AddComponent(Component* component)
{
	int order = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); ++iter)
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
