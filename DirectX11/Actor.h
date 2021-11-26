#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>

class Actor
{
public:
	enum class ActorState
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class BaseScene* scene);
	virtual ~Actor();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	virtual void Bind(class Renderer* renderer);

	void ComputeWorldTransform();
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	void SetActorState(ActorState state) { mState = state; }
	void SetPosition(const DirectX::XMFLOAT3& postion) { mPosition = postion; mRecomputeWorldTransform = true; }
	void SetScale(float scale) { mScale = scale;  mRecomputeWorldTransform = true;}
	void SetRotation(const DirectX::XMFLOAT3& rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }
	void SetTransformCBuffer(class TransformCBuffer* buffer) { mTransformCBuffer = buffer; }

	class BaseScene* GetScene() const { return mScene; }
	const DirectX::XMMATRIX& GetWorldTransform() const { return mWorldTransform; }
	const DirectX::XMFLOAT3& GetRotation() const{ return mRotation; }
	ActorState GetActorState() const { return mState; }

private:
	DirectX::XMMATRIX mWorldTransform;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRotation;
	bool mRecomputeWorldTransform;
	float mScale;
	ActorState mState;
	std::vector<class Component*> mComponents;
	class BaseScene* mScene;
	class TransformCBuffer* mTransformCBuffer;
};
