#pragma once
#include <vector>
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
	void ProcessInput();
	virtual void ActorInput();
	virtual void Bind(class Renderer* renderer);
	virtual void Bind(class Renderer* renderer, float width, float height);
	void ComputeWorldTransform();
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	void SetActorState(ActorState state) { mState = state; }
	void SetPosition(const DirectX::XMFLOAT3& postion) { mPosition = postion; mRecomputeWorldTransform = true; }
	void SetScale(float scale) { mScale = DirectX::XMFLOAT3{ mScale.x*scale,mScale.y*scale ,mScale.z*scale };  mRecomputeWorldTransform = true; }
	void SetScale(const DirectX::XMFLOAT3& scale) { mScale = scale;  mRecomputeWorldTransform = true; }
	void SetRotation(const DirectX::XMFLOAT3& rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }
	void SetTransformCBuffer(class TransformCBuffer* buffer) { mTransformCBuffer = buffer; }
	void SetRecomputeWorldTransform(bool recompute) { mRecomputeWorldTransform = recompute; }
	class BaseScene* GetScene() const { return mScene; }
	const DirectX::XMFLOAT4X4& GetWorldTransform4x4f() const { return mWorldTransform; }
	DirectX::XMMATRIX GetWorldTransform() const { return DirectX::XMLoadFloat4x4(&mWorldTransform); }
	const DirectX::XMFLOAT3& GetPosition() const { return mPosition; }
	const DirectX::XMFLOAT3& GetRotation() const { return mRotation; }
	const DirectX::XMFLOAT3& GetScale() const { return mScale; }
	DirectX::XMFLOAT3& GetScaleWithoutConst() { return mScale; }
	DirectX::XMFLOAT3 GetForward() const { return DirectX::XMFLOAT3{ sinf(mRotation.y),0.0f,cosf(mRotation.y) }; }
	ActorState GetActorState() const { return mState; }

private:
	std::vector<class Component*> mComponents;

	DirectX::XMFLOAT4X4 mWorldTransform;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRotation;
	DirectX::XMFLOAT3 mScale;
	bool mRecomputeWorldTransform;
	ActorState mState;
	class BaseScene* mScene;
	class TransformCBuffer* mTransformCBuffer;
};
